#define Y_INK_PIXEL_IMPL

#include "y/ink/pixel.hpp"

namespace upsylon
{
    namespace Ink
    {
        template <> const uint8_t  Pixel<uint8_t>::Opaque  = 0xff;
        template <> const uint16_t Pixel<uint16_t>::Opaque = 0xffff;
        template <> const uint32_t Pixel<uint32_t>::Opaque = 0xffffffff;
        template <> const float    Pixel<float>::Opaque    = 1.0f;
        template <> const double   Pixel<double>::Opaque   = 1.0;

        template <> const cplx     Pixel<cplx>::Zero(0,0);

        template <> const uint8_t  Pixel<uint8_t>::Zero  = 0;
        template <> const uint16_t Pixel<uint16_t>::Zero = 0;
        template <> const uint32_t Pixel<uint32_t>::Zero = 0;
        template <> const float    Pixel<float>::Zero    = 0.0f;
        template <> const double   Pixel<double>::Zero   = 0.0;

        template <> uint8_t  Pixel<uint8_t>::Inverse(const uint8_t &x) { return 0xff-x; }
        template <> float    Pixel<float>  ::Inverse(const float   &x) { return 1.0f-x; }

    }
}
