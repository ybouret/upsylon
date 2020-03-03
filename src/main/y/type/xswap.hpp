//! \file

#ifndef Y_XSWAP_INCLUDED
#define Y_XSWAP_INCLUDED 1

#include "y/core/loop.hpp"
#include "y/type/no-throw-copy.hpp"

namespace upsylon {


  

#define Y_XSWAP(I) const uint8_t tmp = A[I]; A[I] = B[I]; B[I] = tmp

    template <typename T>
    struct _xswap
    {
        static inline
        void on( T &a, T &b, int2type<true> ) throw()
        {
            const T tmp = a; a=b; b=tmp;
        }

        static inline
        void on( T &a, T &b, int2type<false> ) throw()
        {
            uint8_t *A = (uint8_t *) &a;
            uint8_t *B = (uint8_t *) &b;
            Y_LOOP_FUNC_(sizeof(T),Y_XSWAP,0);
        }
    };


    template <typename T>
    inline void xswap( T &a, T &b ) throw()
    {
        _xswap<T>::on(a,b, int2type< no_throw_copy<T>::value >() );
    }



}

#endif

