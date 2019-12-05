
#include "y/graphic/pixmap.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Graphic {

        namespace Kernel {

            static const char fn[] = "Graphic::Pixmap: ";

            Bitmap * Pixmap:: CheckBitmap(Bitmap *bmp, const size_t depth)
            {
                assert(bmp);
                assert(0==bmp->refcount());
                
                const size_t bmp_depth = bmp->depth;
                if(depth!=bmp_depth)
                {
                    delete bmp;
                    throw exception("%sinvalid Bitmap.depth=%u for Pixmap.detph=%u", fn, unsigned(bmp->depth), unsigned(depth) );
                }

                return bmp;
            }

        }

    }

}


