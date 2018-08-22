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
        typedef pixmap<float>   fpixmap;     //!< floating point pixmap
        typedef pixmap<uint8_t> upixmap;     //!< pixmap of bytes
        typedef pixmap<RGB>     RGBpixmap;   //!< pixmap of packed RGB
        typedef pixmap<RGBA>    RGBApixmap;  //!< pixmap of packed RGBA
        typedef pixmap<YUV>     YUVpixmap;   //!< pixmap of packed YUV
        typedef pixmap<cplx>    zpixmap;     //!< pixmap of complex real
    }

}

#endif
