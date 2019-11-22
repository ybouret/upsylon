//! \file

#ifndef Y_GRAPHIC_PIXMAP_INCLUDED
#define Y_GRAPHIC_PIXMAP_INCLUDED 1

#include "y/graphic/surface.hpp"

namespace upsylon {

    namespace Graphic {


        template <typename T>
        class Pixmap : public Surface
        {
        public:
            explicit Pixmap(const size_t W, const size_t H);

            virtual ~Pixmap() throw()
            {
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Pixmap);
        };

    }
    
}

#endif

