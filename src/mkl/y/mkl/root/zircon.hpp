
//! \file

#ifndef Y_MATH_FCN_ZIRCON_INCLUDED
#define Y_MATH_FCN_ZIRCON_INCLUDED 1

#include "y/mkl/kernel/lu.hpp"
#include "y/mkl/kernel/diag-symm.hpp"
#include "y/mkl/utils.hpp"
#include "y/sequence/arrays.hpp"
#include "y/mkl/kernel/quark.hpp"
#include "y/core/temporary-link.hpp"
#include "y/core/temporary-value.hpp"
#include "y/core/temporary-acquire.hpp"
#include "y/mkl/opt/bracket.hpp"
#include "y/mkl/opt/minimize.hpp"
#include "y/core/ipower.hpp"


namespace upsylon
{
    namespace mkl
    {
        namespace kernel
        {

            //__________________________________________________________________
            //
            //
            //! base class for zircon
            //
            //__________________________________________________________________
            class zircon
            {
            public:
                static const char CLID[];  //!< "zircon"
                bool              verbose; //!< verbosity
                virtual ~zircon() throw(); //!< cleanup

            protected:
                explicit zircon() throw(); //!< setup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(zircon);
            };
        }

        enum zircon_status
        {
            zircon_success,
            zircon_running,
            zircon_failure
        };


        //______________________________________________________________________
        //
        //! inline verbose output
        //______________________________________________________________________
#define Y_ZIRCON_PRINTLN(MSG) \
do { if(this->verbose) { std::cerr << '[' << CLID << ']' << ' ' << MSG << std::endl; } } while(false)

        //______________________________________________________________________
        //
        //
        //! zircon solver
        //
        //______________________________________________________________________
        template <typename T>
        class zircon : public kernel::zircon
        {

        public:
            typedef typename numeric<T>::vector_field ftype;      //!< alias
            typedef typename numeric<T>::jacobian     jtype;      //!< alias
            typedef typename numeric<T>::function     function1d; //!< alias
            typedef lightweight_array<T>              array_type; //!< alias

            //__________________________________________________________________
            //
            //! setup
            //__________________________________________________________________
            inline explicit zircon() :
            nvar(0),
            A(8),
            J(),
            tJ(),
            H0(),
            H(),
            P(),
            tP(),
            grad( A.next() ),
            eigw( A.next() ),
            step( A.next() ),
            Ftry( A.next() ),
            Xtry( A.next() ),
            Fsqr( A.next() ),
            X1(   A.next() ),
            F1(   A.next() ),
            f_(0),
            F_(0),
            X_(0),
            g(this,&zircon::_g)
            {
            }

            //__________________________________________________________________
            //
            //! cleanup
            //__________________________________________________________________
            inline virtual ~zircon() throw()
            {
            }


            //! starting with f(F,X) precomputed
            inline zircon_status cycle3(addressable<T> &F,
                                        addressable<T> &X,
                                        ftype          &f,
                                        jtype          &fjac)
            {
                assert( F.size() == X.size() );
                //--------------------------------------------------------------
                //
                // prepare topology
                //
                //--------------------------------------------------------------
                core::temporary_value<size_t>         nlink(nvar,X.size());
                core::temporary_link<ftype>           flink(f,&f_);
                core::temporary_link<addressable<T> > Xlink(X,&X_);
                core::temporary_link<addressable<T> > Flink(F,&F_);
                core::temporary_acquire<8>            Rlink;

                //--------------------------------------------------------------
                //
                // prepare memory
                //
                //--------------------------------------------------------------

                A.acquire(nvar);
                const T g0 = __g(F);
                if(g0<=0)
                {
                    Y_ZIRCON_PRINTLN("null rms");
                    return zircon_success;
                }

                //--------------------------------------------------------------
                //
                // prepare jacobian
                //
                //--------------------------------------------------------------
                Rlink << J.  make(nvar,nvar);
                Rlink << tJ. make(nvar,nvar);
                Rlink << H0. make(nvar,nvar);
                Rlink << H.  make(nvar,nvar);
                Rlink << P.  make(nvar,nvar);
                Rlink << tP. make(nvar,nvar);

                fjac(J,X);
                Y_ZIRCON_PRINTLN("g0="<<g0);
                Y_ZIRCON_PRINTLN("X="<<X);
                Y_ZIRCON_PRINTLN("F="<<F);
                Y_ZIRCON_PRINTLN("J="<<J);

                //--------------------------------------------------------------
                //
                // prepare gradient and H
                //
                //--------------------------------------------------------------
                tJ.assign_transpose(J);
                quark::mul(grad,tJ,F);
                quark::mmul(H0,tJ,J);

                Y_ZIRCON_PRINTLN("grad="<<grad);
                Y_ZIRCON_PRINTLN("H0="<<H0);

                //--------------------------------------------------------------
                //
                // regularizing by condition monitoring
                //
                //--------------------------------------------------------------
                T   lam = 0;
                int p   = -int(numeric<T>::sqrt_dig);
                while(true)
                {
                    //----------------------------------------------------------
                    //
                    // build approx matrix
                    //
                    //----------------------------------------------------------
                    const T fac = 1.0 + lam;
                    H.assign(H0);
                    for(size_t i=nvar;i>0;--i)
                    {
                        H[i][i] *= fac;
                    }
                    Y_ZIRCON_PRINTLN("H="<<H);

                    //----------------------------------------------------------
                    //
                    // spectral decomposition
                    //
                    //----------------------------------------------------------
                    if( !diag_symm::build(H,eigw,P,sort_eigv_by_module))
                    {
                        Y_ZIRCON_PRINTLN("diagonalize failure");
                        return zircon_failure;
                    }
                    Y_ZIRCON_PRINTLN("w="<<eigw);

                    //----------------------------------------------------------
                    //
                    // guess kernel
                    //
                    //----------------------------------------------------------
                    const size_t ker = __find<T>::truncate(*eigw,nvar);
                    if( ker> 0)
                    {
                        //------------------------------------------------------
                        // zero matrix
                        //------------------------------------------------------
                        if(ker>=nvar)
                        {
                            Y_ZIRCON_PRINTLN("null jacobian");
                            return zircon_failure;
                        }

                        //------------------------------------------------------
                        // singular: try to regularize
                        //------------------------------------------------------
                        if(!increase(lam,p))
                        {
                            return zircon_failure;
                        }
                        continue;
                    }

                    //----------------------------------------------------------
                    //
                    // monitor scaling
                    //
                    //----------------------------------------------------------
                    const T      scaling = fabs_of(eigw[nvar])/fabs_of(eigw[1]);
                    Y_ZIRCON_PRINTLN("scaling="<<scaling);
                    if(scaling<1e-3)
                    {
                        if(!increase(lam,p))
                        {
                            return zircon_failure;
                        }
                        continue;
                    }

                    break;
                }
                Y_ZIRCON_PRINTLN("# <forward with lambda=" << lam << ">" );

                //--------------------------------------------------------------
                //
                // compute trial step
                //
                //--------------------------------------------------------------
                tP.assign_transpose(P);
                {

                    addressable<T> &tPgrad = Fsqr;
                    quark::mul(tPgrad,tP,grad);
                    for(size_t i=nvar;i>0;--i)
                    {
                        tPgrad[i] /= -eigw[i];
                    }
                    quark::mul(step,P,tPgrad);
                }
                Y_ZIRCON_PRINTLN("step="<<step);


                //--------------------------------------------------------------
                //
                // optimize this computation
                //
                //--------------------------------------------------------------
                const T g1 = g(1);
                Y_ZIRCON_PRINTLN("g1="<<g1);


                if(g1>=g0)
                {
                    //----------------------------------------------------------
                    // need to shrink
                    //----------------------------------------------------------
                    triplet<T> U     = { 0,  -1,  1 };
                    triplet<T> G     = { g0, -1, g1 };
                    bracket::inside(g,U,G);
                    const T    u_opt = minimize::run(g,U,G);
                    const T    g_opt = g(u_opt);
                    Y_ZIRCON_PRINTLN("# <shrinking> g(" << u_opt << ")=" << g_opt);
                }
                else
                {
                    quark::set(X1,Xtry);
                    quark::set(F1,Ftry);
                    //----------------------------------------------------------
                    // try inside step
                    //----------------------------------------------------------
                    const T u_ins = numeric<T>::inv_gold;
                    const T g_ins = g(u_ins);
                    Y_ZIRCON_PRINTLN("# <probe:damping> g(" << u_ins << ")=" << g_ins);
                    if(g_ins<=g1)
                    {
                        //------------------------------------------------------
                        // need to damp!
                        //------------------------------------------------------
                        triplet<T> U     = { 0,  u_ins,  1 };
                        triplet<T> G     = { g0, g_ins, g1 };
                        const T    u_opt = minimize::run(g,U,G);
                        const T    g_opt = g(u_opt);
                        Y_ZIRCON_PRINTLN("# <damping> g(" << u_opt << ")=" << g_opt);
                    }
                    else
                    {
                        //------------------------------------------------------
                        // try outside
                        //------------------------------------------------------
                        const T u_ext = numeric<T>::gold;
                        const T g_ext = g(u_ext);
                        Y_ZIRCON_PRINTLN("# <probe:growing> g(" << u_ext << ")=" << g_ext);
                        if(g1<=g_ext)
                        {
                            //--------------------------------------------------
                            // need to relax
                            //--------------------------------------------------
                            triplet<T> U     = { 0,  1,  u_ext };
                            triplet<T> G     = { g0, g1, g_ext };
                            const T    u_opt = minimize::run(g,U,G);
                            const T    g_opt = g(u_opt);
                            Y_ZIRCON_PRINTLN("# <growing> g(" << u_opt << ")=" << g_opt);

                        }
                        else
                        {
                            //--------------------------------------------------
                            // take default step
                            //--------------------------------------------------
                            quark::set(Ftry,F1);
                            quark::set(Xtry,X1);
                        }
                    }
                }

                return converged() ? zircon_success : zircon_running;

            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(zircon);
            size_t           nvar;
            arrays<T>        A;
            matrix<T>        J;
            matrix<T>        tJ;
            matrix<T>        H0;
            matrix<T>        H;
            matrix<T>        P;
            matrix<T>        tP;
            array_type      &grad;
            array_type      &eigw;
            array_type      &step;
            array_type      &Ftry;
            array_type      &Xtry;
            array_type      &Fsqr;
            array_type      &X1;
            array_type      &F1;
            ftype           *f_;
            addressable<T>  *F_;
            addressable<T>  *X_;
            function1d       g;

            //! local computation of |FF|^2/2
            inline T __g(const accessible<T> &FF) const throw()
            {
                assert(FF.size()==nvar);
                for(size_t i=nvar;i>0;--i)
                {
                    Fsqr[i] = square_of(FF[i]);
                }
                hsort(Fsqr,comparison::decreasing<T>);
                T sum = 0;
                for(size_t i=nvar;i>0;--i)
                {
                    sum += Fsqr[i];
                }
                return T(0.5) * sum;
            }

            //! wrapper to g(X+u*step)
            inline T _g(const T u)
            {
                quark::muladd(Xtry,*X_,u,step);
                (*f_)(Ftry,Xtry);
                return __g(Ftry);
            }

            //! increase lambda for regularizing
            bool increase( T &lam, int p)
            {
                static const int pmax = int(numeric<T>::dig);
                if(lam<=0)
                {
                    if(p>=0)
                    {
                        lam = ipower<T>(10,p);
                    }
                    else
                    {
                        lam = ipower<T>(0.1,-p);
                    }
                    Y_ZIRCON_PRINTLN("lam="<<lam);
                    return true;
                }
                else
                {
                    ++p;
                    lam *= 10;
                    if(p <= pmax)
                    {
                        Y_ZIRCON_PRINTLN("lam="<<lam);
                        return true;
                    }
                    else
                    {
                        Y_ZIRCON_PRINTLN("singular topology");
                        return false;
                    }
                }
            }


            //! check local convergence
            inline bool converged() throw()
            {
                const bool xcvg = __find<T>::convergence(*X_,Xtry);
                const bool fcvg = __find<T>::convergence(*F_,Ftry);
                Y_ZIRCON_PRINTLN("# <convergence> variables=" << xcvg << " | functions=" << fcvg);
                return xcvg || fcvg;
            }




        };

    }

}

#endif
