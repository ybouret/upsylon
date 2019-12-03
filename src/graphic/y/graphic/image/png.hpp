//! \file

#ifndef Y_GRAPHIC_IMAGE_IO_INCLUDED
#define Y_GRAPHIC_IMAGE_IO_INCLUDED 1

#include "y/graphic/image.hpp"

namespace upsylon {

    namespace Graphic {

        class PNG_Format : public Image::Format
        {
        public:
            explicit PNG_Format();
            virtual ~PNG_Format() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(PNG_Format);
        };


    }
}

#endif

