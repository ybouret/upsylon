
#include "y/gfx/image/format/png.hpp"
#include "y/gfx/pblock.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace graphic
    {

        png_format:: ~png_format() throw()
        {
        }

        const char png_format::ID[] = "PNG";

        png_format:: png_format() :
        image::named_format(ID, "png")
        {
        }

        bitmap png_format:: load_(const string       &file,
                                  const void         *opts,
                                  const rgba_to_type &conv)
        {

            throw exception("not supported");
        }


        void   png_format:: save_(const bitmap       &bmp,
                                  const string       &file,
                                  const void         *opts,
                                  const type_to_rgba &conv)
        {
            assert(conv.depth()==bmp.depth);
            throw exception("not supported");
        }
    }

}

