
//! \file

#ifndef Y_GRAPHIC_IMAGE_INCLUDED
#define Y_GRAPHIC_IMAGE_INCLUDED 1

#include "y/graphic/bitmap.hpp"
#include "y/graphic/color/rgba2data.hpp"
#include "y/string.hpp"

namespace upsylon {

    namespace Graphic {

        class Image
        {
        public:

            class Format : public Object
            {
            public:
                const string name;

                virtual ~Format() throw();

                virtual Bitmap *load(const string &filename,
                                     const size_t  depth,
                                     RGBA2Data    &proc,
                                     void         *args) const = 0;

            protected:
                explicit Format(const char *id);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Format);
            };

        };


    }
}

#endif

