
//! \file

#ifndef Y_GRAPHIC_CONVERT_INCLUDED
#define Y_GRAPHIC_CONVERT_INCLUDED 1

#include "y/graphic/types.hpp"
#include "y/graphic/color/rgb.hpp"
#include "y/graphic/color/yuv.hpp"

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

            static
            YUV RGB2YUV(const float r, const float g, const float b) throw();

            static inline
            YUV RGB2YUV(const uint8_t r, const uint8_t g, const uint8_t b) throw()
            {
                return RGB2YUV( UnitFloat[r], UnitFloat[g], UnitFloat[b] );
            }

            static inline
            YUV RGB2YUV(const Kernel::RGB<uint8_t> &C ) throw()
            {
                return RGB2YUV(C.r,C.g,C.b);
            }

            static inline
            YUV RGB2YUV(const Kernel::RGBA<uint8_t> &C ) throw()
            {
                return RGB2YUV(C.r,C.g,C.b);
            }


        };

    }

}

#endif
