#define Y_INK_PIXEL_IMPL
#include "y/ink/color/rgb.hpp"

namespace upsylon
{
    namespace Ink
    {
        template <> const RGB  Pixel<RGB>::Zero(0,0,0);
        template <> const RGBA Pixel<RGBA>::Zero(0,0,0,0xff);

        template <> RGB  Pixel<RGB>:: Inverse(const RGB &C ) { return RGB(0xff-C.r,0xff-C.g,0xff-C.b);       }
        template <> RGBA Pixel<RGBA>::Inverse(const RGBA &C) { return RGBA(0xff-C.r,0xff-C.g,0xff-C.b,0xff); }

        template <> bool Pixel<RGB>::  IsZero(const RGB  &C) { return (C.r<=0) && (C.g<=0) && (C.b<=0); }
        template <> bool Pixel<RGBA>:: IsZero(const RGBA &C) { return (C.r<=0) && (C.g<=0) && (C.b<=0); }


        template <> RGB Pixel<RGB>::Average(RGB *arr, const size_t num)
        {

            rgb<float> ave(0,0,0);
            for(size_t i=0;i<num;++i)
            {
                const RGB C = arr[i];
                ave.r += float(C.r);
                ave.g += float(C.g);
                ave.b += float(C.b);

            }
            return RGB( floorf(ave.r/num+0.5f), floorf(ave.g/num+0.5f), floorf(ave.b/num+0.5f) );
        }

        template <> RGBA Pixel<RGBA>::Average(RGBA *arr, const size_t num)
        {

            rgba<float> ave(0,0,0,0);
            for(size_t i=0;i<num;++i)
            {
                const RGBA C = arr[i];
                ave.r += float(C.r);
                ave.g += float(C.g);
                ave.b += float(C.b);
                ave.a += float(C.a);

            }
            return RGBA(uint8_t(floorf(ave.r/num+0.5f)), 
						uint8_t(floorf(ave.g/num+0.5f)),
						uint8_t(floorf(ave.b/num+0.5f)),
						uint8_t(floorf(ave.a/num+0.5f)) );
        }

        template <> int Pixel<RGB>:: Compare( const RGB &lhs, const RGB &rhs )
        {
            return (int(lhs.r)+int(lhs.g)+int(lhs.b))-(int(rhs.r)+int(rhs.g)+int(rhs.b));
        }

        template <> int Pixel<RGBA>:: Compare( const RGBA &lhs, const RGBA &rhs )
        {
            return (int(lhs.r)+int(lhs.g)+int(lhs.b))-(int(rhs.r)+int(rhs.g)+int(rhs.b));
        }

#define __Y_BLEND(FIELD) uint8_t( (unsigned(fg.FIELD) * F + unsigned(bg.FIELD) * B)>>8 )

        template <>
        RGB Pixel<RGB>::Blend( const RGB &fg, const RGB &bg, const uint8_t alpha )
        {
            switch(alpha)
            {
                case 0:    return bg;
                case 0xff: return fg;
                default: break;
            }
            const unsigned F = alpha;
            const unsigned B = 255-F;
            return RGB( __Y_BLEND(r), __Y_BLEND(g), __Y_BLEND(b) );
        }
    }
}

