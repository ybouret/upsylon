
//! \file

#ifndef Y_MATH_FCN_ZIRCON_INCLUDED
#define Y_MATH_FCN_ZIRCON_INCLUDED 1

#include "y/mkl/kernel/lu.hpp"
#include "y/mkl/utils.hpp"
#include "y/sequence/arrays.hpp"
#include "y/mkl/kernel/quark.hpp"
#include "y/core/temporary-link.hpp"
#include "y/core/temporary-value.hpp"
#include "y/comparison.hpp"
#include "y/mkl/opt/bracket.hpp"
#include "y/mkl/opt/minimize.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace kernel
        {
            class zircon
            {
            public:
                static const char CLID[];

                bool              verbose;

                virtual ~zircon() throw();

            protected:
                explicit zircon() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(zircon);
            };
        }

#define Y_ZIRCON_PRINTLN(MSG) \
do { if(this->verbose) { std::cerr << '[' << CLID << ']' << ' ' << MSG << std::endl; } } while(false)

        template <typename T>
        class zircon : public kernel::zircon
        {

        public:
            typedef typename numeric<T>::vector_field ftype; //!< alias
            typedef typename numeric<T>::jacobian     jtype; //!, alias
            typedef typename numeric<T>::function     function1d; //!< alias
            typedef lightweight_array<T>              array_type; //!< alias


            inline explicit zircon() :
            nvar(0),
            A(4),
            J(),
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

        private:
            Y_DISABLE_COPY_AND_ASSIGN(zircon);
            size_t           nvar;
            arrays<T>        A;
            matrix<T>        J;
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



        };

    }

}

#endif
