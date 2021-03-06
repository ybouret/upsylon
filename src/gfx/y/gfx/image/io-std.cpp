

#include "y/gfx/image/io.hpp"
#include "y/gfx/image/format/jpeg.hpp"
#include "y/gfx/image/format/png.hpp"
#include "y/gfx/image/format/tiff.hpp"

namespace upsylon
{

    namespace graphic
    {

        namespace image
        {
#define Y_IMAGE_IO(NAME) do {                          \
/**/ auto_ptr<named_format> fmt = new NAME##_format(); \
/**/ if( !search(fmt->name) ) define( fmt.yield() );   \
} while(false)

            io & io:: standard()
            {
                Y_IMAGE_IO(jpeg);
                Y_IMAGE_IO(png);
                Y_IMAGE_IO(tiff);
                return *this;
            }
        }

    }

}
