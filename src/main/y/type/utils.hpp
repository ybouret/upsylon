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
}

#endif

