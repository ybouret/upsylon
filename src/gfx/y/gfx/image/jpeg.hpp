
//! \file

#ifndef Y_GFX_IMAGE_JPEG_INCLUDED
#define Y_GFX_IMAGE_JPEG_INCLUDED 1

#include "y/gfx/image/named-format.hpp"

namespace upsylon
{
    namespace graphic
    {

        //! using jpeg
        class jpeg_format : public image::named_format
        {
        public:
            virtual ~jpeg_format() throw();
            explicit jpeg_format();

            virtual bitmap load(const string       &file,
                                const void         *opts,
                                const rgba_to_type &conv);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(jpeg_format);
        };

    }

}

#endif
