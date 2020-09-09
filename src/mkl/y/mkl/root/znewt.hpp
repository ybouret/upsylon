
//! \file

#ifndef Y_MATH_FCN_ZNEWT_INCLUDED
#define Y_MATH_FCN_ZNEWT_INCLUDED 1

#include "y/mkl/fcn/jacobian.hpp"
#include "y/mkl/kernel/lu.hpp"
#include "y/mkl/kernel/quark.hpp"

namespace upsylon
{
    namespace mkl
    {

        template <typename T>
        class znewt
        {
        public:

            typedef typename numeric<T>::vector_field field_type;


            inline explicit znewt() : J()
            {
            }

            inline virtual ~znewt() throw()
            {
            }

            //! start from f(F,X)
            inline bool step(addressable<T> &F,
                             addressable<T> &X,
                             field_type     &f,
                             jacobian<T>    &fjac )
            {
                static const T ftol = numeric<T>::ftol;
                assert(F.size()==X.size());

                //--------------------------------------------------------------
                // initialize
                //--------------------------------------------------------------
                const size_t n = X.size();
                //std::cerr << F << "@" << X;
                J.make(n,n);
                fjac(J,X);

                //--------------------------------------------------------------
                // compute Newton's step
                //--------------------------------------------------------------
                if( !LU::build(J) ) return false;
                array<T> &step = J.r_aux1; assert(step.size()==X.size());
                quark::neg(step,F);
                LU::solve(J,step);

                //--------------------------------------------------------------
                // take full step whilst checking convergence
                //--------------------------------------------------------------
                bool converged = true;
                for(size_t i=n;i>0;--i)
                {
                    const T x_old = X[i];
                    const T x_new = (X[i]  += step[i]);
                    const T dx    = fabs_of(x_new-x_old);
                    if( dx > ftol * fabs_of(x_new) ) converged=false;
                }

                f(F,X);
                //std::cerr << " => " << F << "@" << X << std::endl;
                return converged;
            }

            matrix<T> J;


        private:


        };


    }

}

#endif
