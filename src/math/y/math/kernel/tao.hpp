//! \file
#ifndef Y_MATH_TAO_INCLUDED
#define Y_MATH_TAO_INCLUDED 1

#include "y/container/matrix.hpp"
#include "y/concurrent/scheme/simd.hpp"
#include "y/type/bzset.hpp"
#include "y/math/types.hpp"

namespace upsylon
{
    namespace math
    {
        
        //! Templated Algebraic Operations
        struct tao
        {
            ////////////////////////////////////////////////////////////////////
            //
            // level-1 : array ops
            //
            ////////////////////////////////////////////////////////////////////
            
            //__________________________________________________________________
            //
            // ld
            //__________________________________________________________________
#include "tao-ld.hxx"
            //! a[1..size()]=v
            template <typename T> static inline
            void ld( array<T> &a, typename type_traits<T>::parameter_type v, concurrent::for_each *loop=0)
            {
                if(loop)
                {
                    _ld(a,v,*loop);
                }
                else
                {
                    _ld(a,v);
                }
            }
            
            //__________________________________________________________________
            //
            // set
            //__________________________________________________________________
#include "tao-set.hxx"
            //! a[1..size()] = b[1..a.size()]
            template <typename T,typename U> static inline
            void set( array<T> &a, const array<U> &b, concurrent::for_each *loop=0)
            {
                if(loop)
                {
                    _set(a,b,*loop);
                }
                else
                {
                    _set(a,b);
                }
            }

            //! negate all values
            template <typename T,typename U> static inline
            void neg( array<T> &a, const array<U> &b )
            {
                assert(a.size()<=b.size());
                for(size_t i=a.size();i>0;--i)
                {
                    a[i] = -static_cast<T>(b[i]);
                }
            }
            
            //__________________________________________________________________
            //
            // add
            //__________________________________________________________________
#include "tao-add.hxx"
            //! a[1..size()] += b[1..a.size()]
            template <typename T,typename U> static inline
            void add( array<T> &a, const array<U> &b, concurrent::for_each *loop=0)
            {
                if(loop)
                {
                    _add(a,b,*loop);
                }
                else
                {
                    _add(a,b);
                }
            }

            //! a[1..size()] = b[1..a.size()] + c[]
            template <typename T,typename U,typename V> static inline
            void add( array<T> &a, const array<U> &b, const array<V> &c)
            {
                assert(b.size()>=a.size());
                assert(c.size()>=a.size());
                for(size_t i=a.size();i>0;--i)
                {
                    a[i] = static_cast<T>(b[i])+static_cast<T>(c[i]);
                }

            }

            
            //__________________________________________________________________
            //
            // sub
            //__________________________________________________________________
#include "tao-sub.hxx"
            //! a[1..size()] -= b[1..a.size()]
            template <typename T,typename U> static inline
            void sub( array<T> &a, const array<U> &b, concurrent::for_each *loop=0)
            {
                if(loop)
                {
                    _sub(a,b,*loop);
                }
                else
                {
                    _sub(a,b);
                }
            }

            //! a[1..size()] = b[1..a.size()] - c[]
            template <typename T,typename U,typename V> static inline
            void sub( array<T> &a, const array<U> &b, const array<V> &c)
            {
                assert(b.size()>=a.size());
                assert(c.size()>=a.size());
                for(size_t i=a.size();i>0;--i)
                {
                    a[i] = static_cast<T>(b[i])-static_cast<T>(c[i]);
                }

            }

            //__________________________________________________________________
            //
            // muladd
            //__________________________________________________________________
#include "tao-muladd.hxx"
            //! a[1..size()] += x * b[1..a.size()]
            template <typename T,typename U> static inline
            void muladd( array<T> &a, typename type_traits<T>::parameter_type x, const array<U> &b, concurrent::for_each *loop=0)
            {
                if(loop)
                {
                    _muladd(a,x,b,*loop);
                }
                else
                {
                    _muladd(a,x,b);
                }
            }
            
            //__________________________________________________________________
            //
            // mulsub
            //__________________________________________________________________
            //! a[1..size()] -= x * b[1..a.size()]
            template <typename T,typename U> static inline
            void mulsub( array<T> &a, typename type_traits<T>::parameter_type x, const array<U> &b, concurrent::for_each *loop=0)
            {
                if(loop)
                {
                    _mulsub(a,x,b,*loop);
                }
                else
                {
                    _mulsub(a,x,b);
                }
            }
            
            //__________________________________________________________________
            //
            // mulset
            //__________________________________________________________________
            //! a[1..size()] = x*b[1..a.size()]
            template <typename T,typename U> static inline
            void mulset( array<T> &a, typename type_traits<T>::parameter_type x, const array<U> &b, concurrent::for_each *loop=0)
            {
                if(loop)
                {
                    _mulset(a,x,b,*loop);
                }
                else
                {
                    _mulset(a,x,b);
                }
            }
            
            
            //__________________________________________________________________
            //
            // dot product
            //__________________________________________________________________
#include "tao-dot.hxx"
            //! a*b, don't use concurrent version on compound types!
            template <typename T,typename U,typename V> static inline
            T dot( const array<U> &a, const array<V> &b, concurrent::for_each *loop=0)
            {
                assert(a.size()==b.size());
                if(loop)
                {
                    return _dot<T,U,V>(a,b,*loop);
                }
                else
                {
                    return _dot<T,U,V>(a,b);
                }
            }
            
            
            //__________________________________________________________________
            //
            // rms
            //__________________________________________________________________
#include "tao-rms.hxx"
            //! single vector
            template <typename T> static inline
            typename real_for<T>::type rms( const array<T> &a, concurrent::for_each *loop=0)
            {
                if(loop)
                {
                    return _rms<T>(a,*loop);
                }
                else
                {
                    return _rms<T>(a);
                }
            }
            
            //! two vectors
            template <typename T> static inline
            typename real_for<T>::type rms( const array<T> &a, const array<T> &b, concurrent::for_each *loop=0)
            {
                assert(a.size()==b.size());
                if(loop)
                {
                    return _rms<T>(a,b,*loop);
                }
                else
                {
                    return _rms<T>(a,b);
                }
            }
            
            //! single vector
            template <typename T> static inline
            typename real_for<T>::type mod2( const array<T> &a, concurrent::for_each *loop=0)
            {
                if(loop)
                {
                    return _mod2<T>(a,*loop);
                }
                else
                {
                    return _mod2<T>(a);
                }
            }
            
            //! two vectors
            template <typename T> static inline
            typename real_for<T>::type mod2( const array<T> &a, const array<T> &b, concurrent::for_each *loop=0)
            {
                if(loop)
                {
                    return _mod2<T>(a,b,*loop);
                }
                else
                {
                    return _mod2<T>(a,b);
                }
            }
            
            //! b = a + x * step
            template <typename T, typename U, typename V> static inline
            void setprobe( array<T> &b, const array<U> &a, typename type_traits<T>::parameter_type x, const array<V> & step )
            {
                assert(a.size()==b.size());
                assert(a.size()==step.size());

                for(size_t i=b.size();i>0;--i)
                {
                    b[i] = static_cast<T>(a[i]) + x * static_cast<T>(step[i]);
                }
            }

            //! compute the maximum fractional error of |a-b|
            template <typename T> static inline
            T fractional_error( const array<T> &a, const array<T> &b )
            {
                assert(a.size()==b.size());

                T ans = 0;
                for(size_t i=a.size();i>0;--i)
                {
                    const T aa    = a[i];
                    const T bb    = b[i];
                    const T delta = fabs_of(aa-bb);
                    const T err   = (delta+delta)/( fabs_of(aa) + fabs_of(bb) + numeric<T>::tiny );
                    ans = max_of(ans,err);
                }
                return ans;
            }
            ////////////////////////////////////////////////////////////////////
            //
            // level-2 : matrix/array ops
            //
            ////////////////////////////////////////////////////////////////////
#include "tao-mul.hxx"
            
            //! a = M*b
            template <typename T,typename U,typename V> static inline
            void mul( array<T> &a, const matrix<U> &M, const array<V> &b, concurrent::for_each *loop=0)
            {
                assert(M.cols==b.size());
                if(loop)
                {
                    _mul<T,U,V>(a,M,b,*loop);
                }
                else
                {
                    _mul<T,U,V>(a,M,b);
                }
            }
            
            //! a += M*b
            template <typename T,typename U,typename V> static inline
            void mul_add( array<T> &a, const matrix<U> &M, const array<V> &b, concurrent::for_each *loop=0)
            {
                assert(M.cols==b.size());
                if(loop)
                {
                    _mul_add<T,U,V>(a,M,b,*loop);
                }
                else
                {
                    _mul_add<T,U,V>(a,M,b);
                }
            }
            
            //! a -= M*b
            template <typename T,typename U,typename V> static inline
            void mul_sub( array<T> &a, const matrix<U> &M, const array<V> &b, concurrent::for_each *loop=0)
            {
                assert(M.cols==b.size());
                if(loop)
                {
                    _mul_sub<T,U,V>(a,M,b,*loop);
                }
                else
                {
                    _mul_sub<T,U,V>(a,M,b);
                }
            }

            //! A'*S*A, assuming S is SYMMETRIC
            template <typename T,typename U,typename V> static inline
            T quadratic(const matrix<U> &S, const array<V> &A)
            {
                assert(S.rows==S.cols);
                assert(S.rows==A.size());
                const size_t n = S.rows;
                T q(0), p(0);
                for(size_t i=n,im=i-1;i>0;--i,--im)
                {
                    const array<U> &S_i = S[i];
                    const T         A_i = static_cast<T>(A[i]);
                    q += static_cast<T>(S_i[i]) * (A_i*A_i);
                    for(size_t j=im;j>0;--j)
                    {
                        p += static_cast<T>(S_i[j]) * A_i * static_cast<T>(A[j]);
                    }
                }
                return q+(p+p);
            }

            ////////////////////////////////////////////////////////////////////
            //
            // level-3 : matrix/matrix ops
            //
            ////////////////////////////////////////////////////////////////////
#include "tao-mmul.hxx"
            //! M=A*B
            template <typename T,typename U,typename V> static inline
            void mmul(matrix<T>       &M,
                      const matrix<U> &A,
                      const matrix<V> &B, concurrent::for_each *loop=0 )
            {
                assert(M.rows==A.rows);
                assert(A.cols==B.rows);
                assert(M.cols==B.cols);
                if(loop)
                {
                    _mmul(M,A,B,*loop);
                }
                else
                {
                    _mmul(M,A,B);
                }
            }
            
            //! M=A*B'
            template <typename T,typename U,typename V> static inline
            void mmul_rtrn(matrix<T>       &M,
                           const matrix<U> &A,
                           const matrix<V> &B, concurrent::for_each *loop=0 )
            {
                assert(M.rows==A.rows);
                assert(A.cols==B.cols);
                assert(M.cols==B.rows);
                if(loop)
                {
                    _mmul_rtrn(M,A,B,*loop);
                }
                else
                {
                    _mmul_rtrn(M,A,B);
                }
            }
            
        };
        
    }
}

#endif

