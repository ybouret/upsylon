#include "y/ink/color.hpp"

namespace upsylon
{
    namespace ink
    {
        template <> const uint8_t  color<uint8_t>::opaque  = 0xff;
        template <> const uint16_t color<uint16_t>::opaque = 0xffff;
        template <> const uint32_t color<uint32_t>::opaque = 0xffffffff;
        template <> const float    color<float>::opaque    = 1.0f;
        template <> const double   color<double>::opaque   = 1.0;

    }
}
