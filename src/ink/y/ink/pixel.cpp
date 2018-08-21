#define Y_INK_PIXEL_IMPL

#include "y/ink/pixel.hpp"

namespace upsylon
{
    namespace ink
    {
        template <> const uint8_t  pixel<uint8_t>::opaque  = 0xff;
        template <> const uint16_t pixel<uint16_t>::opaque = 0xffff;
        template <> const uint32_t pixel<uint32_t>::opaque = 0xffffffff;
        template <> const float    pixel<float>::opaque    = 1.0f;
        template <> const double   pixel<double>::opaque   = 1.0;

        template <> const cplx     pixel<cplx>::zero(0,0);

        template <> const uint8_t  pixel<uint8_t>::zero  = 0;
        template <> const uint16_t pixel<uint16_t>::zero = 0;
        template <> const uint32_t pixel<uint32_t>::zero = 0;
        template <> const float    pixel<float>::zero    = 0.0f;
        template <> const double   pixel<double>::zero   = 0.0;

    }
}
