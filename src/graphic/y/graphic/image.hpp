
//! \file

#ifndef Y_GRAPHIC_IMAGE_INCLUDED
#define Y_GRAPHIC_IMAGE_INCLUDED 1

#include "y/graphic/bitmap.hpp"
#include "y/graphic/color/rgba2data.hpp"
#include "y/graphic/color/data2rgba.hpp"
#include "y/string.hpp"

namespace upsylon {

    namespace Graphic {


        //! Images and Formats management
        class Image
        {
        public:

            class Format : public Object
            {
            public:
                const string name; //!< name for database of formats

                virtual ~Format() throw(); //!< cleanup

                //! load a new bitmap
                virtual Bitmap *load(const string &filename,
                                     const size_t  depth,
                                     RGBA2Data    &proc,
                                     const string *params) const = 0;

                //! save a bitmap
                virtual void    save(const string &filename,
                                     const Bitmap &bmp,
                                     Data2RGBA    &proc,
                                     const string *params) const = 0;

            protected:
                explicit Format(const char *id); //!< setup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Format);
            };


        };


    }
}

#endif

