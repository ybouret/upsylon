
//! \file


#ifndef Y_GRAPHIC_COLOR_CONVERT_INCLUDED
#define Y_GRAPHIC_COLOR_CONVERT_INCLUDED 1

#include "y/gfx/color/rgb.hpp"
#include "y/gfx/color/yuv.hpp"


namespace upsylon
{
    namespace GFX
    {

        struct Convert
        {

            static inline uint8_t ToByte(const float f) throw()
            {
                assert(f>=0.0f); assert(f<=1.0f);
                return floorf(f*255.0f+0.5f);
            }


            //! convertion of a triplet to a byte
            static uint8_t RGB2GS(const uint8_t r, const uint8_t g, const uint8_t b) throw();


            static const rgbF YScale; //!<  0.299     0.587     0.114
            static const rgbF UScale; //!< -0.147    -0.289     0.436
            static const rgbF VScale; //!<  0.615    -0.515    -0.100

            static float RGB2Y(const float   r, const float   g, const float   b) throw();
            static float RGB2U(const float   r, const float   g, const float   b) throw();
            static float RGB2V(const float   r, const float   g, const float   b) throw();
            static float RGB2Y(const uint8_t r, const uint8_t g, const uint8_t b) throw();
            static float RGB2U(const uint8_t r, const uint8_t g, const uint8_t b) throw();
            static float RGB2V(const uint8_t r, const uint8_t g, const uint8_t b) throw();

            template <typename COLOR> static inline
            YUV RGB2YUV(const COLOR &c) throw()
            {
                return YUV( RGB2Y(c.r,c.g,c.b),  RGB2U(c.r,c.g,c.b),  RGB2V(c.r,c.g,c.b) );
            }

            static float YUV2R( const YUV & ) throw(); //!< YUV->red,   float
            static float YUV2G( const YUV & ) throw(); //!< YUV->green, float
            static float YUV2B( const YUV & ) throw(); //!< YUV->blue,  float

            template <typename T> static
            Kernel::rgb<T> YUV2RGB( const YUV & ) throw();




            static const float UnitFloat[256];
            

        };

    }

}

#endif
