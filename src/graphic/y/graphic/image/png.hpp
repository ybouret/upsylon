//! \file

#ifndef Y_GRAPHIC_IMAGE_IO_INCLUDED
#define Y_GRAPHIC_IMAGE_IO_INCLUDED 1

#include "y/graphic/image.hpp"

namespace upsylon {

    namespace Graphic {

        //! using libpng
        class PNG_Format : public Image::Format
        {
        public:
            explicit PNG_Format();         //!< "PNG"
            virtual ~PNG_Format() throw(); //!< cleanup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(PNG_Format);
        };


    }
}

#endif

