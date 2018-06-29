//! \file
#ifndef Y_TYPE_UTILS_INCLUDED
#define Y_TYPE_UTILS_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{
    //! default minimum
    template <typename T> inline T min_of( const T a, const T b ) { return (a<b) ? a:b; }

    //! default maximum
    template <typename T> inline T max_of( const T a, const T b ) { return (b<a) ? a:b; }

    //! default absolute value
    template <typename T> inline T abs_of(const T a) { return (a<T(0)) ? -a : a; }

    //! default sign value
    template <typename T> inline int sign_of( const T a ) { return (a<0) ? -1 : (  (0<a) ? 1 : 0 );     }

    //! default clamping of value
    template <typename T> inline T clamp( const T amin, const T a, const T amax ) { return (a<amin) ? amin : ( (amax<a) ? amax : a );     }

    //! default squared value
    template <typename T> inline T square_of( const T a ) { return a*a; }

    //! power of two detection
    template <typename T> inline bool is_a_power_of_two( T v ) throw()
    {
        static const T one(1);
        return (v != 0) && !( v & (v - one) );
    }

    //! next power of two \todo check overflow
    template <typename T>
    inline T next_power_of_two( T v ) throw()
    {
        assert(v>0);
        --v;
        for( size_t shift = 1; shift <= (sizeof(T)<<2); shift <<= 1 )
        {
            v |= ( v >> shift );
        }
        return ++v;
    }
    
}

#endif

