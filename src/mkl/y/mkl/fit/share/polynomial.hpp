//! \file

#ifndef Y_MKL_FIT_POLY_INCLUDED
#define Y_MKL_FIT_POLY_INCLUDED 1

#include "y/mkl/fit/share/common.hpp"
#include "y/mkl/fit/sample.hpp"
#include "y/core/ipower.hpp"
#include "y/mkl/kernel/lu.hpp"

namespace upsylon {

    namespace mkl {

        namespace Fit {

            //! setup a polynomial fit function
            template <typename T>
            class Polynomial : public Common
            {
            public:
                typedef typename Type<T>::Matrix Matrix; //!< alias
                typedef typename Type<T>::Vector Vector; //!< alias

                //! setup with n=degree+1 coefficients
                inline explicit Polynomial(const size_t degree, const char *pfx) :
                Common( degree+1  )
                {
                    sequence<string> &ids    = aliasing::_(names);
                    const string      prefix = pfx;
                    for(size_t i=0;i<=degree;++i)
                    {
                        const string id     = prefix + Suffix(i);
                        ids.push_back(id);
                    }
                    assert(1+degree==names.size());
                }

                //! cleanup
                inline virtual ~Polynomial() throw()
                {
                }


                //! return evaluation of polynomial
                inline T compute(const T              x,
                                 const accessible<T> &aorg,
                                 const Variables     &vars) const
                {
                    assert( names.size() > 0 );
                    size_t n = names.size();
                    T      ans = vars(aorg,names[n--]);
                    while(n>0)
                    {
                        ans *= x;
                        ans += vars(aorg,names[n--]);
                    }

                    return ans;
                }

                //! initialize using linear least squares
                inline bool initialize(Sample<T>              &sample,
                                       addressable<T>         &aorg,
                                       const accessible<bool> &used,
                                       const Variables        &vars)
                {
                    const size_t         N = sample.count();
                    const size_t         n = names.size();
                    const accessible<T> &X = *sample.abscissa;
                    const accessible<T> &Y = *sample.ordinate;
                    addressable<T>      &Z = *sample.adjusted;

                    Matrix       mu(n,n);
                    Vector       rhs(n,0);
                    for(size_t k=n;k>0;--k)
                    {
                        const string   &id  = names[k];
                        T              &rhk = rhs[k];
                        addressable<T> &muk = mu[k];
                        if( vars(used,id) )
                        {
                            for(size_t i=N;i>0;--i)
                            {
                                const T xi  = X[i];
                                const T yi  = Y[i];
                                const T xik = ipower<T>(xi,k-1);
                                rhk += yi*xik;
                                for(size_t l=k;l>0;--l)
                                {
                                    if(vars(used,names[l]))
                                    {
                                        muk[l] += xik * ipower<T>(xi,l-1);
                                    }
                                }
                            }
                        }
                        else
                        {
                            mu[k][k] = 1;
                            rhk      = vars(aorg,id);
                        }
                    }
                    Type<T>::Regularize(mu);

                    if( !LU::build(mu) )
                    {
                        quark::ld(Z,0);
                        return false;
                    }
                    else
                    {
                        LU::solve(mu,rhs);
                        for(size_t k=n;k>0;--k)
                        {
                            vars( aorg, names[k] ) = rhs[k];
                        }
                        for(size_t i=N;i>0;--i)
                        {
                            Z[i] = compute(X[i], aorg, vars);
                        }
                        return true;
                    }
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Polynomial);
            };


        }

    }
}

#endif
