
#include "y/gfx/image.hpp"
#include "y/gfx/image/jpeg.hpp"

namespace upsylon
{

    namespace graphic
    {

#define Y_IMAGE_IO(NAME) do {                          \
/**/ auto_ptr<named_format> fmt = new NAME##_format(); \
/**/ if( !search(fmt->name) ) define( fmt.yield() );   \
} while(false)

        void image::io:: standard()
        {
            Y_IMAGE_IO(jpeg);
        }
    }

}
