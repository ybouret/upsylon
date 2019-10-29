
//!\file

#ifndef Y_CORE_IPOWER_INCLUDED
#define Y_CORE_IPOWER_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    //! integer power
    template <typename T>
    inline T   ipower( T x, size_t n) throw()
    {
        T ans(1);
        while( n != 0 )
        {
            if( (n & 1) != 0)
            {
                ans *= x;
                if (n == 1) return ans;
            }
            x *= x;
            n >>= 1;
        }
        return ans;
    }


}

#endif

