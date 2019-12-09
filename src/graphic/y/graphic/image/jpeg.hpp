
//! \file

#ifndef Y_GRAPHIC_IMAGE_JPEG_INCLUDED
#define Y_GRAPHIC_IMAGE_JPEG_INCLUDED 1

#include "y/graphic/image.hpp"

namespace upsylon {

    namespace Graphic {

        //! using libjpeg
        class JPEG_Format : public Image::Format
        {
        public:
            explicit JPEG_Format();         //!< "PNG"
            virtual ~JPEG_Format() throw(); //!< cleanup

            virtual Bitmap *load(const string         &filename,
                                 const size_t         depth,
                                 RGBA2Data            &proc,
                                 const Image::Options *params) const;

            virtual void    save(const string         &filename,
                                 const Bitmap         &bmp,
                                 Data2RGBA            &proc,
                                 const Image::Options *params ) const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(JPEG_Format);
        };


    }
}

#endif
