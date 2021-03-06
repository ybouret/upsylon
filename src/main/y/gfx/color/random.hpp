

#ifndef Y_COLOR_RANDOM_INCLUDED
#define Y_COLOR_RANDOM_INCLUDED 1

#include "y/randomized/bits.hpp"

namespace upsylon
{
    
    namespace graphic
    {
        //! helper to create random color
        struct random_color
        {
            //! defined for uint8_t, float, rgb, rgba, YUV
            template <typename T> static
            T get( randomized::bits & ) throw();
        };
    }
    
}

#endif

