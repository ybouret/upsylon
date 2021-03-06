//! \file

#ifndef Y_GRAPHIC_IMAGE_PNG_INCLUDED
#define Y_GRAPHIC_IMAGE_PNG_INCLUDED 1

#include "y/graphic/image.hpp"

namespace upsylon {

    namespace Graphic {

        //! using libpng
        class PNG_Format : public Image::Format
        {
        public:
            explicit PNG_Format();         //!< "PNG"
            virtual ~PNG_Format() throw(); //!< cleanup

            virtual Bitmap *load(const string         &filename,
                                 const size_t         depth,
                                 RGBA2Data            &proc,
                                 const Image::Options *params) const;

            virtual void    save(const string         &filename,
                                 const Bitmap         &bmp,
                                 Data2RGBA            &proc,
                                 const Image::Options *params ) const;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(PNG_Format);
        };


    }
}

#endif

