#include "y/core/isqrt.hpp"

namespace upsylon {

    const uint8_t isqrt::table[65536] =
    {
#include "isqrt.inc"
    };

}

