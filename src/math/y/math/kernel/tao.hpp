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
            void mmul_rtrn(matrix<T>  &M,
                      const matrix<U> &A,
                      const matrix<V> &B, concurrent::for_each *loop=0 )
            {
                assert(M.rows==A.rows);
                assert(A.cols==B.rows);
                assert(M.cols==B.cols);
                if(loop)
                {
                    //_mmul(M,A,B,*loop);
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

