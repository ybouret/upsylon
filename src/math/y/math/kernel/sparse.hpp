#ifndef Y_MATH_KERNEL_SPARSE_INCLUDED
#define Y_MATH_KERNEL_SPARSE_INCLUDED 1

#include "y/sparse/matrix.hpp"
#include "y/sparse/array.hpp"

namespace upsylon
{

    namespace math
    {

        //! SPArse Reusable Kernel
        struct spark
        {


            //! a += b;
            template <typename T, typename U> static inline
            void set( sparse_array<T> &a, const sparse_array<U> &b )
            {
                assert(a.size()==b.size());
                a.ldz();
                a.core.ensure( b.core.size() );
                typename sparse_array<U>::const_iterator i = b.begin();
                for(size_t n=b.core.size();n>0;--n,++i)
                {
                    a[i.key()] = ***i;
                }
            }

            //! a += b;
            template <typename T, typename U> static inline
            void add( sparse_array<T> &a, const sparse_array<U> &b )
            {
                assert(a.size()==b.size());
                typename sparse_array<U>::const_iterator i = b.begin();
                for(size_t n=b.core.size();n>0;--n,++i)
                {
                    a[i.key()] += ***i;
                }
            }

            //! a -= b;
            template <typename T, typename U> static inline
            void sub( sparse_array<T> &a, const sparse_array<U> &b )
            {
                assert(a.size()==b.size());
                typename sparse_array<U>::const_iterator i = b.begin();
                for(size_t n=b.core.size();n>0;--n,++i)
                {
                    a[i.key()] -= ***i;
                }
            }

            //! dot product
            template <typename T, typename U> static inline
            T dot( const sparse_array<T> &a, const sparse_array<U> &b )
            {
                T ans = 0;
                size_t                                   na = a.core.size();
                typename sparse_array<T>::const_iterator ia = a.begin();
                while(na>0)
                {
                    const U *pb = b( ia.key() );
                    if(pb)
                    {
                        ans += (***ia) * (*pb);
                    }
                    --na; ++ia;
                }
                return ans;
            }

            //! norm squared
            template <typename T> static inline
            T norm2( const sparse_array<T> &a )
            {
                size_t na = a.core.size();
                if(na>0)
                {
                    typename sparse_array<T>::const_iterator ia = a.begin();
                    T ans = square_of( ***ia );
                    while(--na>0)
                    {
                        ++ia;
                        ans += square_of( ***ia );
                    }
                    return ans;
                }
                else
                {
                    return 0;
                }
            }

            //! a *= x;
            template <typename T> static inline
            void mul( sparse_array<T> &a, typename type_traits<T>::parameter_type x )
            {
                for(typename sparse_array<T>::iterator i=a.begin();i!=a.end();++i)
                {
                    ***i *= x;
                }
            }

            //! a += x*b;
            template <typename T, typename U> static inline
            void mul_add( sparse_array<T> &a, typename type_traits<T>::parameter_type x, const sparse_array<U> &b )
            {
                assert(a.size()==b.size());
                typename sparse_array<U>::const_iterator i = b.begin();
                for(size_t n=b.core.size();n>0;--n,++i)
                {
                    a[i.key()] += x * (***i);
                }
            }

            //! a -= x*b;
            template <typename T, typename U> static inline
            void mul_sub( sparse_array<T> &a, typename type_traits<T>::parameter_type x, const sparse_array<U> &b )
            {
                assert(a.size()==b.size());
                typename sparse_array<U>::const_iterator i = b.begin();
                for(size_t n=b.core.size();n>0;--n,++i)
                {
                    a[i.key()] -= x * (***i);
                }
            }


            
            //! matrix vector product
            template <typename T, typename U, typename V> static inline
            void mul( sparse_array<T> &a, const sparse_matrix<U> &M, const sparse_array<V> &b )
            {
                assert(a.size()==M.rows);
                assert(b.size()==M.cols);
                a.ldz();
                typename sparse_matrix<U>::const_iterator k = M.begin();
                for(size_t m=M.core.size();m>0;--m,++k)
                {
                    const sparse::matrix_key &K = k.key();
                    const V                  *B = b(K.c);  if(!B) continue;
                    a[K.r] += (***k) * (*B);
                }
            }

            //! matrix vector product, transpose
            template <typename T, typename U, typename V> static inline
            void mul_trn( sparse_array<T> &a, const sparse_matrix<U> &M, const sparse_array<V> &b )
            {
                assert(a.size()==M.cols);
                assert(b.size()==M.rows);
                a.ldz();
                typename sparse_matrix<U>::const_iterator k = M.begin();
                for(size_t m=M.core.size();m>0;--m,++k)
                {
                    const sparse::matrix_key &K = k.key();
                    const V                  *B = b(K.r);  if(!B) continue;
                    a[K.c] += (***k) * (*B);
                }
            }




        };

    }
}

#endif

