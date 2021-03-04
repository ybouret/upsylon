
//! \file

#ifndef Y_GFX_IMAGE_JPEG_INCLUDED
#define Y_GFX_IMAGE_JPEG_INCLUDED 1

#include "y/gfx/image.hpp"

namespace upsylon
{
    namespace graphic
    {

        class jpeg_format : public image::named_format
        {
        public:
            virtual ~jpeg_format() throw();
            explicit jpeg_format();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(jpeg_format);
        };

    }

}

#endif
