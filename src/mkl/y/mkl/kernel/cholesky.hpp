//! \file
#ifndef Y_MKL_CHOLESKY_INCLUDED
#define Y_MKL_CHOLESKY_INCLUDED 1

#include "y/container/matrix.hpp"
#include "y/mkl/types.hpp"

namespace upsylon {

    namespace math {

        //! Cholesky decomposition
        struct cholesky
        {

            //! decomposition of positive defined matrix
            /**
             only the upper part of a is required, and
             the result is stored in the lower triangle,
             put the diagonal into diag.
             a MUST have some extra memory
             */
            template <typename T> static inline
            bool build( matrix<T> &a ) throw()
            {
                assert(a.rows==a.cols);
                const size_t          n    = a.rows;
                lightweight_array<T> &diag = a.r_aux1;
                for(size_t i=1;i<=n;++i)
                {
                    addressable<T> &a_i = a[i];
                    for(size_t j=i;j<=n;++j )
                    {
                        addressable<T> &a_j = a[j];
                        T sum = a_i[j];
                        for( size_t k=i-1;k>0;--k)
                        {
                            sum -= a_i[k] * a_j[k];
                        }

                        if( i == j )
                        {
                            if( sum <= 0 )
                                return false;
                            diag[i] = sqrt_of( sum );
                        }
                        else
                        {
                            a_j[i] = sum / diag[i];
                        }
                    }
                }

                for(size_t i=n;i>0;--i)
                {
                    a[i][i] = diag[i];
                    for(size_t j=n;j>i;--j)
                    {
                        a[i][j] = 0;
                    }
                }
                
                return true;
            }

            //! solve a*x=b, a was decomposed...
            template <typename T> static inline
            void solve( array<T> &x, const matrix<T> &a, const array<T> &b) throw()
            {
                assert(a.rows==a.cols);
                assert(x.size()>=a.rows);
                assert(b.size()>=a.rows);
                const size_t n = a.rows;

                for( size_t i=1; i<=n; ++i )
                {
                    T sum = b[i];
                    for( size_t k=i-1; k>0;--k )
                        sum -= a[i][k] * x[k];
                    x[i] = sum / a[i][i];
                }

                for( size_t i=n; i>0; --i )
                {
                    T sum = x[i];
                    for( size_t k=i+1; k <=n; ++k )
                        sum -= a[k][i] * x[k];
                    x[i] = sum / a[i][i];
                }

            }

        };

    }
}

#endif
