
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
            static const float   UnitFloat[256];

            //! conversion of bytes to triplet helper
            static const uint8_t GreyScaleTable[3*255+1];

            //! triplet to byte conversion
            static
            uint8_t GreyScale(const uint8_t r, const uint8_t g, const uint8_t b) throw();

            //! use r,g,b of a color
            template <typename COLOR>
            static inline
            uint8_t GreyScale(const COLOR &C) throw()
            {
                return GreyScale(C.r,C.g,C.b);
            }


            //! floating point conversion
            static
            YUV RGB2YUV(const float r, const float g, const float b) throw();

            //! bytes conversion
            static inline
            YUV RGB2YUV(const uint8_t r, const uint8_t g, const uint8_t b) throw()
            {
                return RGB2YUV( UnitFloat[r], UnitFloat[g], UnitFloat[b] );
            }

            //! r,g,b part of a color conversion
            template <typename COLOR>
            static inline
            YUV RGB2YUV(const COLOR &C ) throw()
            {
                return RGB2YUV(C.r,C.g,C.b);
            }

            static float YUV2R( const YUV & ) throw(); //!< YUV->red,   float
            static float YUV2G( const YUV & ) throw(); //!< YUV->green, float
            static float YUV2B( const YUV & ) throw(); //!< YUV->blue,  float

            //! YUV->rgb
            static inline rgb YUV2RGB( const YUV &_ ) throw()
            {
                return rgb(Float2Byte( YUV2R(_) ),
                           Float2Byte( YUV2G(_) ),
                           Float2Byte( YUV2B(_) ) );
            }




            //! internal conversions
            template <typename TARGET,typename SOURCE> static
            TARGET Get( const SOURCE & ) throw();



            //! for stack operations
            template <typename T> static
            T FloatTo( const float ) throw();
        };

    }

}

#endif
