
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
            A(5),
            J(),
            step( A.next() ),
            Ftry( A.next() ),
            Xtry( A.next() ),
            Fsqr( A.next() ),
            f_(0),
            F_(0),
            X_(0),
            g(this, & zircon::_g)
            {
            }

            inline virtual ~zircon() throw()
            {
            }

            //! starting with f(F,X) precomputed
            bool cycle( addressable<T> &F, addressable<T> &X, ftype &f, jtype &fjac )
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



                A.acquire(nvar);
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

                quark::neg(step,F);
                LU::solve(J,step);
                Y_ZIRCON_PRINTLN("step = "<<J);

                const T    g0 = __g(F);
                if(g0<=0)
                {
                    Y_ZIRCON_PRINTLN("null rms");
                    return true;
                }
                const T    g1 = g(1);
                triplet<T> U  = { 0,   1,  1 };
                triplet<T> G  = { g0, g1, g1 };

                Y_ZIRCON_PRINTLN("g0   = "<<g0);
                Y_ZIRCON_PRINTLN("g1   = "<<g1);

                if(g1<g0)
                {
                    bracket::expand(g,U,G);
                }
                else
                {
                    bracket::inside(g,U,G);
                }
                const T lambda = max_of<T>(minimize::run(g,U,G),0);
                const T gm     = g(lambda);
                Y_ZIRCON_PRINTLN("lam  = "<<lambda);
                Y_ZIRCON_PRINTLN("gm   = "<<gm);

                // check convergence
                const bool xcvg = __find<T>::convergence(X,Xtry);
                const bool fcvg = __find<T>::convergence(F,Ftry);

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

            inline T __g(const accessible<T> &F) const throw()
            {
                assert(F.size()==nvar);
                for(size_t i=nvar;i>0;--i)
                {
                    Fsqr[i] = square_of(F[i]);
                }
                hsort(Fsqr,comparison::decreasing<T>);
                T sum = 0;
                for(size_t i=nvar;i>0;--i)
                {
                    sum += Fsqr[i];
                }
                return T(0.5) * sum;
            }

            inline T _g(const T lambda)
            {
                quark::muladd(Xtry,*X_,lambda,step);
                (*f_)(Ftry,Xtry);
                return __g(Ftry);
            }



        };

    }

}

#endif
