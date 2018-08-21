#define Y_INK_PIXEL_IMPL
#include "y/ink/color/rgb.hpp"

namespace upsylon
{
    namespace ink
    {
        template <> const RGB  pixel<RGB>::zero(0,0,0);
        template <> const RGBA pixel<RGBA>::zero(0,0,0,0xff);
    }
}

