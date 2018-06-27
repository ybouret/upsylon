#ifndef Y_TYPE_UTILS_INCLUDED
#define Y_TYPE_UTILS_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{
    template <typename T>
    inline T min_of( const T a, const T b )
    {
        return (a<b) ? a:b;
    }
    
    template <typename T>
    inline T max_of( const T a, const T b )
    {
        return (b<a) ? a:b;
    }
    
    template <typename T>
    inline T abs_of(const T a)
    {
        return (a<T(0)) ? -a : a;
    }

    template <typename T>
    inline int sign_of( T a )
    {
        return (a<0) ? -1 : (  (0<a) ? 1 : 0 );
    }

    template <typename T>
    inline T clamp( T amin, T a, T amax )
    {
        return (a<amin) ? amin : ( (amax<a) ? amax : a );
    }

    template <typename T>
    inline T square_of( T a )
    {
        return a*a;
    }

    template <typename T>
    inline bool is_a_power_of_two( T v ) throw()
    {
        static const T one(1);
        return (v != 0) && !( v & (v - one) );
    }

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

