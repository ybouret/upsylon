
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
#include "y/comparison.hpp"
#include "y/mkl/opt/bracket.hpp"
#include "y/mkl/opt/minimize.hpp"
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
            A(6),
            J(),
            grad( A.next() ),
            eigw( A.next() ),
            step( A.next() ),
            Ftry( A.next() ),
            Xtry( A.next() ),
            Fsqr( A.next() ),
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
            bool cycle( addressable<T> &F, addressable<T> &X, ftype &f, jtype &fjac )
            {
                static const T lambda_min = 0.1;
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

                //--------------------------------------------------------------
                //
                // prepare jacobian
                //
                //--------------------------------------------------------------
                A.acquire(nvar);
                const T g0 = __g(F);
                if(g0<=0)
                {
                    Y_ZIRCON_PRINTLN("null rms");
                    return true;
                }
                J.make(nvar,nvar);
                fjac(J,X);

                Y_ZIRCON_PRINTLN("<cycle>");
                Y_ZIRCON_PRINTLN("X    = "<<X);
                Y_ZIRCON_PRINTLN("F    = "<<F);
                Y_ZIRCON_PRINTLN("J    = "<<J);

                if(!LU::build(J))
                {
                    Y_ZIRCON_PRINTLN("singular jacobian");
                    return false;
                }

                //--------------------------------------------------------------
                //
                // compute full newton's step
                //
                //--------------------------------------------------------------
                quark::neg(step,F);
                LU::solve(J,step);
                Y_ZIRCON_PRINTLN("step = "<<step);


                //--------------------------------------------------------------
                //
                // local minimize
                //
                //--------------------------------------------------------------
                const T    g1 = g(1);
                triplet<T> U  = { 0,   1,  1 };
                triplet<T> G  = { g0, g1, g1 };

                Y_ZIRCON_PRINTLN("g0   = "<<g0);
                Y_ZIRCON_PRINTLN("g1   = "<<g1);

                T lambda = 1;
                if(g1<g0)
                {
                    Y_ZIRCON_PRINTLN("expand");
                    U.c = numeric<T>::gold;
                    G.c = g(U.c);
                    if(G.c<g1)
                    {
                        // still decreasing, don't go too fast!
                    }
                    else
                    {
                        // little backtrack
                        bracket::inside(g,U,G);
                        lambda = minimize::run(g,U,G);
                    }
                }
                else
                {
                    Y_ZIRCON_PRINTLN("inside");
                    // constrained backtrack
                    bracket::inside(g,U,G);
                    lambda = max_of<T>(minimize::run(g,U,G),lambda_min);
                }
                const T gm     = g(lambda);
                Y_ZIRCON_PRINTLN("lam  = "<<lambda);
                Y_ZIRCON_PRINTLN("gm   = "<<gm);
                Y_ZIRCON_PRINTLN("Xtry = "<<Xtry);
                Y_ZIRCON_PRINTLN("Ftry = "<<Ftry);

                //--------------------------------------------------------------
                //
                // check convergence
                //
                //--------------------------------------------------------------
                const bool xcvg = __find<T>::convergence(X,Xtry);
                const bool fcvg = __find<T>::convergence(F,Ftry);
                Y_ZIRCON_PRINTLN("xcvg = "<<xcvg);
                Y_ZIRCON_PRINTLN("fcvg = "<<fcvg);

                return xcvg||fcvg;
            }

            //! starting with f(F,X) precomputed
            inline zircon_status cycle2(addressable<T> &F,
                                        addressable<T> &X,
                                        ftype &f,
                                        jtype &fjac)
            {
                //static const T lambda_min = 0.1;
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

                //--------------------------------------------------------------
                //
                // prepare jacobian
                //
                //--------------------------------------------------------------

                A.acquire(nvar);
                const T g0 = __g(F);
                if(g0<=0)
                {
                    Y_ZIRCON_PRINTLN("null rms");
                    return zircon_success;
                }
                J.make(nvar,nvar);
                tJ.make(nvar,nvar);
                H.make(nvar,nvar);
                P.make(nvar,nvar);
                tP.make(nvar,nvar);
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
                quark::mmul(H,tJ,J);

                Y_ZIRCON_PRINTLN("grad="<<grad);
                Y_ZIRCON_PRINTLN("H="<<H);


                if(!diag_symm::build(H,eigw,P,sort_eigv_by_module))
                {
                    Y_ZIRCON_PRINTLN("unable to diagonalize");
                    return zircon_failure;
                }
                Y_ZIRCON_PRINTLN("P="<<P);
                Y_ZIRCON_PRINTLN("w="<<eigw);
                tP.assign_transpose(P);
                const size_t ker = __find<T>::truncate(*eigw,nvar);
                Y_ZIRCON_PRINTLN("ker="<<ker);

                if(ker<=0)
                {
                    if(ker>=nvar)
                    {
                        Y_ZIRCON_PRINTLN("null jacobian");
                        return zircon_failure;
                    }
                    else
                    {
                        return tryRegularStep(g0);
                    }

                }
                else
                {
                    return trySingularStep(g0,ker);
                }


             }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(zircon);
            size_t           nvar;
            arrays<T>        A;
            matrix<T>        J;
            matrix<T>        tJ;
            matrix<T>        H;
            matrix<T>        P;
            matrix<T>        tP;
            array_type      &grad;
            array_type      &eigw;
            array_type      &step;
            array_type      &Ftry;
            array_type      &Xtry;
            array_type      &Fsqr;
            ftype           *f_;
            addressable<T>  *F_;
            addressable<T>  *X_;
            function1d       g;

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

            inline T _g(const T u)
            {
                quark::muladd(Xtry,*X_,u,step);
                (*f_)(Ftry,Xtry);
                return __g(Ftry);
            }

            inline bool converged() throw()
            {

                const bool xcvg = __find<T>::convergence(*X_,Xtry);
                const bool fcvg = __find<T>::convergence(*F_,Ftry);
                std::cerr  << *X_ << "=>" << Xtry << std::endl;
                std::cerr  << *F_ << "=>" << Ftry << std::endl;

                Y_ZIRCON_PRINTLN("# <convergence> variables=" << xcvg << " | functions=" << fcvg);
                return xcvg || fcvg;
            }


            zircon_status tryRegularStep(const T g0)
            {
                //--------------------------------------------------------------
                // compute the newton's step from decomposition
                //--------------------------------------------------------------
                array_type &tmp = Fsqr;
                quark::mul(tmp,tP,grad);
                const T coherence = eigw[nvar]/eigw[1];
                Y_ZIRCON_PRINTLN("# <regular step> coherence="<<coherence);
                for(size_t i=nvar;i>0;--i)
                {
                    tmp[i] /= -eigw[i];
                }
                quark::mul(step, P, tmp);
                Y_ZIRCON_PRINTLN("step="<<step);

                //--------------------------------------------------------------
                // check where we landed
                //--------------------------------------------------------------
                const T    g1 = g(1);
                Y_ZIRCON_PRINTLN("g1="<<g1);
                {
                    ios::ocstream fp("zircon.dat");
                    for(int i=-100;i<=200;++i)
                    {
                        const T x = T(i)/100;
                        fp("%g %g\n", x, g(x) );
                    }
                }



                if(g1<g0)
                {
                    const T u_ins = numeric<T>::inv_gold;
                    const T g_ins = g(u_ins);
                    Y_ZIRCON_PRINTLN("#decreasing");
                    Y_ZIRCON_PRINTLN("# <probing> g_ins=" << g_ins << "@" << u_ins);
                    if(g_ins<=g1)
                    {
                        triplet<T> U    = { 0,  u_ins,  1 };
                        triplet<T> G    = { g0, g_ins, g1 };
                        const T    uopt = minimize::run(g,U,G);
                        const T    gopt = g(uopt);
                        Y_ZIRCON_PRINTLN("# <shrinking> g(" << uopt <<")=" << gopt);
                    }
                    else
                    {
                        const T u_out = numeric<T>::gold;
                        const T g_out = g(u_out);
                        Y_ZIRCON_PRINTLN("# <probing> g_out=" << g_out << "@" << u_out);
                        if(g_out>=g1)
                        {

                            triplet<T> U    = { 0,   1, u_out };
                            triplet<T> G    = { g0, g1, g_out };
                            const T    uopt = minimize::run(g,U,G);
                            const T    gopt = g(uopt);
                            Y_ZIRCON_PRINTLN("# <expanding> g(" << uopt <<")=" << gopt);
                        }
                        else
                        {
                            Y_ZIRCON_PRINTLN("# <full step>");
                            (void) g(1);
                        }
                    }
                }
                else
                {
                    assert(g1>=g0);
                    Y_ZIRCON_PRINTLN("#backtracking");
                    triplet<T> U = { 0,  -1,  1 };
                    triplet<T> G = { g0, -1, g1 };
                    bracket::inside(g,U,G);
                    const T    uopt = minimize::run(g,U,G);
                    const T    gopt = g(uopt);
                    Y_ZIRCON_PRINTLN("#g(" << uopt <<")=" << gopt);
                }

                return converged() ? zircon_success : zircon_running;
            }


            zircon_status trySingularStep(const T g0, const size_t ker)
            {
                const size_t img = nvar - ker;
                Y_ZIRCON_PRINTLN("# <singular step> dim(Ker)=" << ker << " | dim(Img)=" << img);

                matrix<T> S(nvar,img);
                matrix<T> Z(nvar,ker);

                {
                    const size_t bytes = img * sizeof(T);
                    for(size_t k=nvar;k>0;--k)
                    {
                        memcpy( &S[k][1], &P[k][1], bytes);
                    }
                }

                {
                    const size_t bytes = ker * sizeof(T);
                    for(size_t k=nvar;k>0;--k)
                    {
                        memcpy( &Z[k][1], &P[k][1+img], bytes);
                    }
                }

                Y_ZIRCON_PRINTLN("S=" << S);
                Y_ZIRCON_PRINTLN("Z=" << Z);


                return zircon_failure;
            }

        };

    }

}

#endif
