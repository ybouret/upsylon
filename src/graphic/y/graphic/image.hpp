
//! \file

#ifndef Y_GRAPHIC_IMAGE_INCLUDED
#define Y_GRAPHIC_IMAGE_INCLUDED 1

#include "y/graphic/bitmap.hpp"
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



            protected:
                explicit Format(const char *id);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Format);
            };

        };


    }
}

#endif

