//!\file

#ifndef Y_CORE_ISQRT_INCLUDED
#define Y_CORE_ISQRT_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    //! integer square root
    template <typename T> inline
    T isqrt(const T n) throw()
    {
        if(n<=1)
        {
            return n;
        }
        else
        {
            T x0 = n;
            T x1 = (n>>1);
            while(true)
            {
                x0 = x1;
                x1 = ((x0+n/x0)>>1);
                if(x1>=x0)
                {
                    return x0;
                }
            }
        }

    }

}

#endif

