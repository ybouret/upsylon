
//! \file

#ifndef Y_GRAPHIC_CONVERT_INCLUDED
#define Y_GRAPHIC_CONVERT_INCLUDED 1

#include "y/graphic/types.hpp"

namespace upsylon {

    namespace Graphic {

        //! pixel conversion routines
        struct Convert
        {
            //! assuming 0<=f<=1, return the corresponding closest byte
            static inline uint8_t Float2Byte( const float f ) throw()
            {
                return uint8_t( floorf( 255.0f * f + 0.5f) );
            }

            //! [0..255]/255
            static const float UnitFloat[256];
        };

    }

}

#endif
