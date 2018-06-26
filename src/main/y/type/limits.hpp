//! \file
#ifndef Y_TYPE_LIMITS_INCLUDED
#define Y_TYPE_LIMITS_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{
    template <typename> struct limits_of;
    
    template <>
    struct limits_of<int8_t>
    {
        static const int8_t minimum = -127;
        static const int8_t maximum =  128;
    };
    
    template <>
    struct limits_of<uint8_t>
    {
        static const uint8_t minimum = 0;
        static const uint8_t maximum = 0xff;
    };
    
}

#endif

