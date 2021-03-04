
#include "y/gfx/image/jpeg.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace graphic
    {

        jpeg_format:: ~jpeg_format() throw()
        {
        }

        jpeg_format:: jpeg_format() :
        image::named_format( "JPG", "jpe?g" )
        {
        }

        bitmap jpeg_format:: load(const string       &file,
                                  const void         *opts,
                                  const rgba_to_type &conv)
        {

            throw exception("not implemented");
        }

    }

}


