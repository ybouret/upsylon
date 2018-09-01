#define Y_INK_PIXEL_IMPL
#include "y/ink/color/rgb.hpp"

namespace upsylon
{
    namespace Ink
    {
        template <> const RGB  Pixel<RGB>::Zero(0,0,0);
        template <> const RGBA Pixel<RGBA>::Zero(0,0,0,0xff);

        template <> RGB  Pixel<RGB>:: Inverse(const RGB &C ) { return RGB(0xff-C.r,0xff-C.g,0xff-C.b);       }
        template <> RGBA Pixel<RGBA>::Inverse(const RGBA &C) { return RGBA(0xff-C.r,0xff-C.g,0xff-C.b,0xff); }

    }
}

