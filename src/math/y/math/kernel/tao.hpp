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

        };

    }
}

#endif

