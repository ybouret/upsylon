
//! \file
#ifndef Y_MATH_FIT_POLYNOMIAL_INCLUDED
#define Y_MATH_FIT_POLYNOMIAL_INCLUDED 1

#include "y/math/fit/samples.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {

            //! wrappers for fit with Gaussians
            struct Polynomial
            {

                //! create the name for n variables (a)
                static void Create( Variables &vars, const size_t n);

                //! compute the sum of unerlying gaussians
                template <typename T> static inline
                T Compute( T x, const array<double> &aorg, const Variables &vars )
                {
                    assert( aorg.size() == vars.size() );
                    T ans = 0;
                    for(size_t i=vars.size();i>0;)
                    {
                        const T a = aorg[i];
                        --i;
                        ans += a * ipower(x,i);
                    }
                    return ans;
                }

                //! initialize with moments using simple sample
                template <typename T> static inline
                void Initialize(array<double>      &aorg,
                                const array<bool>   &used,
                                Sample<T>           &sample)
                {
                    assert(aorg.size()==used.size());
                    const size_t n = aorg.size();
                    const size_t N = sample.X.size();
                    matrix<double> mu(n,n);
                    vector<double> rhs(n);
                    for(size_t i=1;i<=n;++i)
                    {
                        for(size_t j=i;j<=n;++j)
                        {
                            const size_t k    = (i-1)+(j-1);

                            T ans = 0;
                            for(size_t l=N;l>0;--l)
                            {
                                ans += ipower( sample.X[l], k);
                            }
                            mu[i][j]  = mu[j][i] = ans;
                        }

                        T r = 0;
                        for(size_t l=N;l>0;--l)
                        {
                            r += sample.Y[l] * ipower(sample.X[l],i-1);
                        }
                        rhs[i] = r;
                    }
                }

            };

        }
    }
}

#endif
