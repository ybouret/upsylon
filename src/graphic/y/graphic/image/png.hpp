//! \file

#ifndef Y_GRAPHIC_IMAGE_IO_INCLUDED
#define Y_GRAPHIC_IMAGE_IO_INCLUDED 1

#include "y/graphic/image.hpp"

namespace upsylon {

    namespace Graphic {

        //! using libpng
        class PNG_Format : public ImageFormat
        {
        public:
            explicit PNG_Format();         //!< "PNG"
            virtual ~PNG_Format() throw(); //!< cleanup

            virtual Bitmap *load(const string &filename,
                                 const size_t  depth,
                                 RGBA2Data    &proc,
                                 void         *args) const;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(PNG_Format);
        };


    }
}

#endif

