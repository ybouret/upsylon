//! \file
#ifndef Y_MATH_FIT_GAUSSIANS_INCLUDED
#define Y_MATH_FIT_GAUSSIANS_INCLUDED 1

#include "y/math/gaussian.hpp"
#include "y/math/fit/variables.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {

            //! wrappers for fit with Gaussians
            struct Gaussians
            {

                //! create the name for n gaussians (a,mu,sigma)
                static void Create( Variables &vars, const size_t n);

                //! compute the sum of unerlying gaussians
                template <typename T> static inline
                T Compute( T x, const array<double> &aorg, const Variables &vars )
                {
                    const size_t ng = vars.size()/3;
                    assert(aorg.size()==vars.size());
                    assert(ng*3==vars.size());
                    size_t j=0;
                    T      ans=0;
                    for(size_t i=1;i<=ng;++i)
                    {
                        const T a    = aorg[++j];
                        const T mu   = aorg[++j];
                        const T sig  = aorg[++j];
                        const T sig2 = sig*sig;
                        const T dx = x-mu;
                        const T den2 = sig2+sig2;
                        ans += a*__exp(-(dx*dx)/den2)/sqrt_of(numeric<T>::pi*den2);
                    }
                    return ans;
                }


            };

        }
    }
}

#endif

