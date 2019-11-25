
//! \file

#ifndef Y_GRAPHIC_PIXMAPS_INCLUDED
#define Y_GRAPHIC_PIXMAPS_INCLUDED 1

#include "y/graphic/pixmap.hpp"
#include "y/graphic/color/rgb.hpp"
#include "y/graphic/color/yuv.hpp"

namespace upsylon {

    namespace Graphic {

        typedef Pixmap<uint8_t> Pixmap1;
        typedef Pixmap<float>   PixmapF;
        typedef Pixmap<double>  PixmapD;
        typedef Pixmap<RGB>     Pixmap3;
        typedef Pixmap<RGBA>    Pixmap4;


    }
}


#endif
