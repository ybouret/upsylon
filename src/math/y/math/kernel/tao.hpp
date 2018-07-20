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
#include "tao-ld.hxx"
            //! a[1..size()]=v
            template <typename T> static void
            ld( array<T> &a, typename type_traits<T>::parameter_type v, concurrent::for_each *loop )
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

        };

    }
}

#endif

