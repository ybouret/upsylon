//! \file
#ifndef Y_MKL_LU_INCLUDED
#define Y_MKL_LU_INCLUDED 1

#include "y/mkl/types.hpp"
#include "y/container/matrix.hpp"
#include "y/type/xnumeric.hpp"

namespace upsylon {

    namespace mkl {

        //! LU decomposition and application
        struct LU
        {
            //! build LU decomposition using matrix extra data
            template <typename T> static inline
            bool build( matrix<T> &a, bool *dneg=0 ) throw()
            {
                //______________________________________________________________
                //
                // sanity check
                //______________________________________________________________
                assert(a.rows>0);
                assert(a.is_square);

                //______________________________________________________________
                //
                // local types
                //______________________________________________________________
                typedef typename real_for<T>::type      scalar_type;
                typedef lightweight_array<scalar_type>  scalar_array;

                //______________________________________________________________
                //
                // initialize
                //______________________________________________________________
                const size_t    n       = a.rows;
                array<size_t>  &indx    = a.r_indices; assert(indx.size()==n);
                array<T>       &extra   = a.r_aux1;    assert(extra.size()==n);
                scalar_array    scal( (scalar_type *)extra.ro(), n );
                if(dneg) *dneg=false;

                //______________________________________________________________
                //
                // initializing scaling/pivot
                //______________________________________________________________
                const scalar_type &amin = xnumeric<scalar_type>::abs_minimum();
                const scalar_type  one  = 1;
                for( size_t i=n;i>0;--i)
                {
                    const accessible<T> & a_i = a[i];
                    scalar_type           piv = 0;

                    for( size_t j=n;j>0;--j)
                    {
                        const scalar_type tmp = fabs_of( a_i[j] );
                        if (tmp>piv)
                        {
                            piv = tmp;
                        }
                    }
                    if( piv <= amin )
                    {
                        return false; //! singular or ill-conditioned matrix
                    }
                    scal[i] = one/piv;
                }

                //______________________________________________________________
                //
                // Crout's algorithm
                //______________________________________________________________
                for(size_t j=1;j<=n;++j)
                {
                    for(size_t i=1;i<j;++i)
                    {
                        addressable<T> &a_i = a[i];
                        T               sum = a_i[j];
                        for(size_t k=1;k<i;++k)
                        {
                            sum -= a_i[k]*a[k][j];
                        }
                        a_i[j]=sum;
                    }

                    scalar_type piv  = 0;
                    size_t      imax = j;
                    for( size_t i=j;i<=n;++i)
                    {
                        addressable<T> &a_i = a[i];
                        T               sum = a_i[j];
                        for(size_t k=1;k<j;++k)
                        {
                            sum -= a_i[k]*a[k][j];
                        }
                        a_i[j]=sum;

                        const scalar_type tmp = scal[i]*fabs_of(sum);
                        if( tmp >= piv )
                        {
                            piv  = tmp;
                            imax = i;
                        }
                    }

                    assert( imax> 0 );
                    if (j != imax)
                    {
                        a.swap_rows( j, imax );
                        if(dneg)
                        {
                            *dneg = ! *dneg;
                        }
                        scal[imax]=scal[j];
                    }

                    indx[j]=imax;

                    if( fabs_of(a[j][j]) <= amin )
                    {
                        return false; //!< singular or ill-conditioned matrix
                    }

                    if (j != n)
                    {
                        const T fac = one/(a[j][j]);
                        for(size_t i=j+1;i<=n;++i)
                        {
                            a[i][j] *= fac;
                        }
                    }
                }

#if !defined(NDEBUG)
                for( size_t i=1; i <= n; ++i )
                {
                    assert( indx[i] >  0 );
                    assert( indx[i] <= n );
                }
#endif

                return true;
            }

            //! solve a vector using a previously LU build matrix
            template <typename T>
            static inline
            void solve( const matrix<T> &a, addressable<T> &b ) throw()
            {
                assert(a.cols>0);
                assert(a.rows>0);
                assert(a.is_square);
                assert(b.size()>=a.rows);

                const size_t    n = a.rows;

                //______________________________________________________________
                //
                // first pass
                //______________________________________________________________
                {
                    const array<size_t> &indx = a.r_indices;
                    for(size_t i=1;i<=n;++i)
                    {
                        const size_t         ip  = indx[i]; assert(ip>0);assert(ip<=n);
                        T                    sum = b[ip];
                        const accessible<T> &a_i = a[i];
                        b[ip] = b[i];
                        for(size_t j=1;j<i;++j)
                        {
                            sum -= a_i[j] * b[j];
                        }
                        b[i] = sum;
                    }
                }

                //______________________________________________________________
                //
                // second pass
                //______________________________________________________________
                {
                    for(size_t i=n;i>0;--i)
                    {
                        const accessible<T>  &a_i = a[i];
                        T                     sum = b[i];
                        for(size_t j=i+1;j<=n;++j)
                        {
                            sum -= a_i[j]*b[j];
                        }
                        b[i]=sum/a_i[i];
                    }

                }
            }

            //! solve a matrix using a previously LU built matrix
            template <typename T>
            static inline
            void solve( const matrix<T> &a, matrix<T> &m )
            {
                const size_t n = m.rows;
                array<T>    &b = m.r_aux1;
                for(size_t j=m.cols;j>0;--j)
                {
                    for(size_t i=n;i>0;--i) b[i]   = m[i][j];
                    solve(a,b);
                    for(size_t i=n;i>0;--i) m[i][j] = b[i];
                }
            }

            //! compute the inverse of a previously LU built matrix
            template <typename T>
            static inline
            void inverse( const matrix<T> &a, matrix<T> &I )
            {
                assert( a.rows == I.rows );
                assert( a.cols == I.cols );
                assert( I.is_square      );
                const T __zero = 0;
                const T __one  = 1;
                const size_t n = I.rows;
                array<T>    &b = I.r_aux1;
                for(size_t j=I.cols;j>0;--j)
                {
                    for(size_t i=n;i>0;--i) b[i] = __zero;
                    b[j] = __one;
                    solve(a,b);
                    for(size_t i=n;i>0;--i) I[i][j] = b[i];
                }
            }

        };

    }
}

#endif
