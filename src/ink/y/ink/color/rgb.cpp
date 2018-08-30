#define Y_INK_PIXEL_IMPL
#include "y/ink/color/rgb.hpp"

namespace upsylon
{
    namespace Ink
    {
        template <> const RGB  Pixel<RGB>::Zero(0,0,0);
        template <> const RGBA Pixel<RGBA>::Zero(0,0,0,0xff);
    }
}

