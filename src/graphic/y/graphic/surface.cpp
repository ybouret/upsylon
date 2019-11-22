
#include "y/graphic/surface.hpp"

namespace upsylon {

    namespace Graphic {

        Surface:: ~Surface() throw()
        {
        }

        Surface:: Surface( Bitmap *bmp ) throw() :
        Bitmap::Pointer(bmp)
        {
        }
        
        Surface:: Surface( const Surface &surface ) throw() :
        Bitmap::Pointer(surface)
        {
        }


    }

}
