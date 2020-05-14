
//!\file

#ifndef Y_CORE_IPOWER_INCLUDED
#define Y_CORE_IPOWER_INCLUDED 1

#include "y/type/traits.hpp"

namespace upsylon {

    //! integer power
    template <typename T>
    inline T   ipower( typename type_traits<T>::parameter_type _x, size_t n) throw()
    {
        T p(1);
        T x(_x);
        while( n != 0 )
        {
            if( (n & 1) != 0)
            {
                p *= x;
                if (n == 1) return p;
            }
            x *= x;
            n >>= 1;
        }
        return p;
    }


}

#endif

