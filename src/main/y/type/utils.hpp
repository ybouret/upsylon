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

#if 0
    inline size_t length_of(const char *s) throw()
    {
        if(!s)
        {
            return 0;
        }
        else
        {
            const char *i = s;
            while( *(i++) )
                ;
            return static_cast<size_t>(i-s);
        }
    }
#endif
    
}

#endif

