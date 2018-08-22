
#include "y/ink/image/jpeg.hpp"
#include "y/ink/image/png.hpp"
#include "y/ink/image/tiff.hpp"

namespace upsylon
{
    namespace  ink
    {
        image &image:: initialize()
        {
            (void) create<jpeg_format>();
            (void) create<png_format>();
            (void) create<tiff_format>();
            return *this;
        }
    }
}

