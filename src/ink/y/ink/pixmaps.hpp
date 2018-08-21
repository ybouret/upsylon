
//! \file

#ifndef Y_INK_PIXMAPS_INCLUDED
#define Y_INK_PIXMAPS_INCLUDED 1

#include "y/ink/pixmap.hpp"
#include "y/ink/color/rgb.hpp"
#include "y/ink/color/yuv.hpp"

namespace upsylon
{
    namespace ink
    {
        typedef pixmap<float>   fpixmap;
        typedef pixmap<uint8_t> upixmap;
        typedef pixmap<RGB>     RGBpixmap;
        typedef pixmap<RGBA>    RGBApixmap;
        typedef pixmap<YUV>     YUVpixmap;
        typedef pixmap<cplx>    zpixmap;
    }

}

#endif
