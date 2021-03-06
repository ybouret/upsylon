

#ifndef Y_COLOR_RANDOM_INCLUDED
#define Y_COLOR_RANDOM_INCLUDED 1

#include "y/randomized/bits.hpp"

namespace upsylon
{
    
    namespace graphic
    {
        struct random_color
        {
            template <typename T> static
            T get( randomized::bits & ) throw();
        };
    }
    
}

#endif

