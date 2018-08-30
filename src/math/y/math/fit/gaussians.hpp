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

            struct Gaussians
            {

                static void Create( Variables &vars, const size_t n);
                
                template <typename T> static inline
                T Compute( T x, const array<double> &aorg, const Variables &vars )
                {
                    const size_t n  = aorg.size(); assert(n==vars.size()); assert(n%3==0);
                    const size_t ng = n/3;
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

