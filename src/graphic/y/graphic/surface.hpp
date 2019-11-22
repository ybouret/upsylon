//! \file

#ifndef Y_GRAPHIC_SURFACE_INCLUDED
#define Y_GRAPHIC_SURFACE_INCLUDED 1

#include "y/graphic/bitmap.hpp"

namespace upsylon {

    namespace Graphic {

        //! smart pointer for bitmap
        class Surface : public Bitmap::Pointer
        {
        public:
            Surface( Bitmap *bmp ) throw();
            virtual ~Surface() throw();
            Surface( const Surface &surface ) throw();
            
        private:
            Y_DISABLE_ASSIGN(Surface);
        };

    }

}

#endif

