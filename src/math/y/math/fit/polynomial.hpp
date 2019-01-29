
//! \file
#ifndef Y_MATH_FIT_POLYNOMIAL_INCLUDED
#define Y_MATH_FIT_POLYNOMIAL_INCLUDED 1

#include "y/math/fit/ls.hpp"
#include "y/math/polynomial.hpp"
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
                T Compute( T x, const array<double> &aorg, const Variables & )
                {
                    return polynomial::eval(x,aorg);
                }

                //! initialize with moments using simple sample
                template <typename T> static inline
                void Initialize(array<T>   &aorg,
                                Sample<T> &sample)
                {
                    const size_t   n = aorg.size();
                    const size_t   N = sample.X.size();
                    matrix<double> mu(n,n);
                    for(size_t i=1,im=0;i<=n;++i,++im)
                    {
                        for(size_t j=i,jm=im;j<=n;++j,++jm)
                        {
                            const size_t k    = im+jm;
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
                            r += sample.Y[l] * ipower(sample.X[l],im);
                        }
                        aorg[i] = r;
                    }
                    if(! LU::build(mu) )
                    {
                        throw exception("Fit::Polynomial::Initialize(singular distribution)");
                    }
                    LU::solve(mu,aorg);
                    for(size_t i=N;i>0;--i)
                    {
                        sample.Yf[i] = polynomial::eval(sample.X[i],aorg);
                    }
                }

                //! fit using polynomial evaluation
                template <typename T> static inline
                void FitWith(LeastSquares<T> &LS,
                             array<T>        &aorg,
                             array<T>        &aerr,
                             array<bool>     &used,
                             Sample<T>       &sample)
                {
                    Create(sample.variables,aorg.size());
                    Initialize(aorg,sample);
                    typename Fit::Type<T>::CFunction f = Fit::Polynomial::Compute<double>;
                    typename Fit::Type<T>::Function  F(f);
                    if(!LS.fit(sample, F, aorg, aerr, used))
                    {
                        throw exception("Fit::Polynomial::Unexpected Algebraic Failure");
                    }
                }

            };



        }
    }
}

#endif
