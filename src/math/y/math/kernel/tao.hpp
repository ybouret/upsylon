//! \file
#ifndef Y_MATH_TAO_INCLUDED
#define Y_MATH_TAO_INCLUDED 1

#include "y/container/matrix.hpp"
#include "y/sequence/array.hpp"
#include "y/concurrent/scheme/simd.hpp"

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
            void ld( array<T> &a, typename type_traits<T>::parameter_type v, concurrent::for_each *loop )
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
            void set( array<T> &a, const array<U> &b, concurrent::for_each *loop )
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
            void add( array<T> &a, const array<U> &b, concurrent::for_each *loop )
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
            void sub( array<T> &a, const array<U> &b, concurrent::for_each *loop )
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
            void muladd( array<T> &a, typename type_traits<T>::parameter_type x, const array<U> &b, concurrent::for_each *loop )
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

            //! a[1..size()] -= x * b[1..a.size()]
            template <typename T,typename U> static inline
            void mulsub( array<T> &a, typename type_traits<T>::parameter_type x, const array<U> &b, concurrent::for_each *loop )
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

            //! a[1..size()] = x*b[1..a.size()]
            template <typename T,typename U> static inline
            void mulset( array<T> &a, typename type_traits<T>::parameter_type x, const array<U> &b, concurrent::for_each *loop )
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
            // sub
            //__________________________________________________________________
#include "tao-dot.hxx"
            //! a*b
            template <typename T,typename U,typename V> static inline
            T dot( const array<U> &a, const array<V> &b, concurrent::for_each *loop )
            {
                if(loop)
                {
                    return _dot<T,U,V>(a,b,*loop);
                }
                else
                {
                    return _dot<T,U,V>(a,b);
                }
            }

        };

    }
}

#endif

