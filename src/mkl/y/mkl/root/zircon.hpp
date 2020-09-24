
//! \file

#ifndef Y_MATH_FCN_ZIRCON_INCLUDED
#define Y_MATH_FCN_ZIRCON_INCLUDED 1

#include "y/mkl/utils.hpp"
#include "y/mkl/kernel/lu.hpp"
#include "y/mkl/kernel/eigen.hpp"
#include "y/mkl/kernel/quark.hpp"
#include "y/mkl/kernel/preconditioning.hpp"

#include "y/sequence/arrays.hpp"
#include "y/core/temporary-link.hpp"
#include "y/core/temporary-value.hpp"
#include "y/core/temporary-acquire.hpp"
#include "y/mkl/opt/bracket.hpp"
#include "y/mkl/opt/minimize.hpp"
#include "y/core/ipower.hpp"

#include "y/ios/ocstream.hpp"




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

        //! status to perform cycle
        enum zircon_status
        {
            zircon_success, //!< converged/blocked
            zircon_running, //!< may run again
            zircon_failure  //!< singularity
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
            A(10),
            J(),
            tJ(),
            C0(),
            C(),
            P(),
            grad(  A.next()  ),
            w(     A.next()  ),
            step(  A.next()  ),
            diag(  A.next()  ),
            omega( A.next() ),
            Ftry(  A.next()  ),
            Xtry(  A.next()  ),
            Fsqr(  A.next()  ),
            X1(    A.next()  ),
            F1(    A.next()  ),
            f_(0),
            F_(0),
            X_(0),
            g(this,&zircon::_g),
            precond()
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
            inline zircon_status operator()(addressable<T> &F,
                                            addressable<T> &X,
                                            ftype          &f,
                                            jtype          &fjac)
            {
                assert( F.size() == X.size() );
                //--------------------------------------------------------------
                //
                // prepare context
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

                Rlink << A.acquire(nvar);
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
                Rlink << C0. make(nvar,nvar);
                Rlink << C.  make(nvar,nvar);
                Rlink << P.  make(nvar,nvar);

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
                quark::mmul(C0,tJ,J);

                Y_ZIRCON_PRINTLN("grad="<<grad);
                Y_ZIRCON_PRINTLN("C0="<<C0);


                //--------------------------------------------------------------
                //
                // curvature pre-conditioner
                //
                //--------------------------------------------------------------
                T   quality = 0;
                if(!precond(omega,C0,NULL,quality))
                {
                    Y_ZIRCON_PRINTLN("singular pre-condition");
                    return zircon_failure;
                }
                Y_ZIRCON_PRINTLN("omega="<<omega);
                Y_ZIRCON_PRINTLN("quality="<<quality);

                const T minimal_q = quality / 1000;


                T   lam = 0;
                int p   = -int(numeric<T>::sqrt_dig);

                while(true)
                {
                    //----------------------------------------------------------
                    //
                    // build approx matrix
                    //
                    //----------------------------------------------------------
                    C.assign(C0);
                    for(size_t i=nvar;i>0;--i)
                    {
                        C[i][i] *= (1.0+omega[i]*lam);
                    }


                    //----------------------------------------------------------
                    //
                    // spectral decomposition
                    //
                    //----------------------------------------------------------
                    if( !eigen::build(C,w,P,sort_eigv_by_module))
                    {
                        Y_ZIRCON_PRINTLN("diagonalize failure");
                        return zircon_failure;
                    }



                    //----------------------------------------------------------
                    //
                    // guess kernel
                    //
                    //----------------------------------------------------------
                    const size_t ker = __find<T>::truncate(*w,nvar);
                    if(ker>0)
                    {
                        Y_ZIRCON_PRINTLN("ker="<<ker);

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
                    // monitor qualitu
                    //
                    //----------------------------------------------------------
                    const T q = w[nvar]/w[1];
                    if(q<minimal_q)
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
                Y_ZIRCON_PRINTLN("C="<<C);
                Y_ZIRCON_PRINTLN("w="<<w);
                Y_ZIRCON_PRINTLN("q="<<w[nvar]/w[1]);


                //--------------------------------------------------------------
                //
                // compute (quasi-)Newton trial step
                //
                //--------------------------------------------------------------
                {

                    addressable<T> &tPgrad = Fsqr;
                    quark::mul_trn(tPgrad,P,grad);
                    for(size_t i=nvar;i>0;--i)
                    {
                        tPgrad[i] /= -w[i];
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
                    push_trial();
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
                            pull_trial();
                        }
                    }
                }

                return converged() ? zircon_success : zircon_running;

            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(zircon);
            size_t             nvar;   //!< num variables
            arrays<T>          A;      //!< arrays memory
            matrix<T>          J;      //!< Jacobian
            matrix<T>          tJ;     //!< Jacobian transpose
            matrix<T>          C0;     //!< initial  curvature
            matrix<T>          C;      //!< modified curvature
            matrix<T>          P;      //!< otrhonormal matrix for eigen
            array_type        &grad;
            array_type        &w;
            array_type        &step;
            array_type        &diag;
            array_type        &omega;
            array_type        &Ftry;
            array_type        &Xtry;
            array_type        &Fsqr;
            array_type        &X1;
            array_type        &F1;
            ftype             *f_;
            addressable<T>    *F_;
            addressable<T>    *X_;
            function1d         g;
            preconditioning<T> precond;


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




            inline void push_trial()
            {
                for(size_t i=nvar;i>0;--i)
                {
                    F1[i] = Ftry[i];
                    X1[i] = Xtry[i];
                }
            }

            inline void pull_trial()
            {
                for(size_t i=nvar;i>0;--i)
                {
                    F1[i] = Ftry[i];
                    X1[i] = Xtry[i];
                }
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
                    return true;
                }
                else
                {
                    ++p;
                    lam *= 10;
                    if(p <= pmax)
                    {
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
