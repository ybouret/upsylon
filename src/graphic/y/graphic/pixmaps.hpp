
//! \file

#ifndef Y_GRAPHIC_PIXMAPS_INCLUDED
#define Y_GRAPHIC_PIXMAPS_INCLUDED 1

#include "y/graphic/pixmap.hpp"
#include "y/graphic/color/rgb.hpp"
#include "y/graphic/color/yuv.hpp"

namespace upsylon {

    namespace Graphic {

        typedef Pixmap<uint8_t> Pixmap1; //!< alias
        typedef Pixmap<float>   PixmapF; //!< alias
        typedef Pixmap<double>  PixmapD; //!< alias
        typedef Pixmap<RGB>     Pixmap3; //!< alias
        typedef Pixmap<RGBA>    Pixmap4; //!< alias


    }
}


#endif
