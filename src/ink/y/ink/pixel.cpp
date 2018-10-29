#define Y_INK_PIXEL_IMPL

#include "y/ink/pixel.hpp"
#include "y/comparison.hpp"

namespace upsylon
{
    namespace Ink
    {
        template <> const uint8_t  Pixel<uint8_t>::  Opaque = 0xff;
        template <> const uint16_t Pixel<uint16_t>:: Opaque = 0xffff;
        template <> const uint32_t Pixel<uint32_t>:: Opaque = 0xffffffff;
        template <> const float    Pixel<float>::    Opaque = 1.0f;
        template <> const double   Pixel<double>::   Opaque = 1.0;

        template <> const cplx     Pixel<cplx>::Zero(0,0);

        template <> const uint8_t  Pixel<uint8_t>::  Zero  = 0;
        template <> const uint16_t Pixel<uint16_t>:: Zero = 0;
        template <> const uint32_t Pixel<uint32_t>:: Zero = 0;
        template <> const float    Pixel<float>::    Zero = 0.0f;
        template <> const double   Pixel<double>::   Zero = 0.0;

        template <> uint8_t  Pixel<uint8_t>::Inverse(const uint8_t &x) { return 0xff-x; }
        template <> float    Pixel<float>  ::Inverse(const float   &x) { return 1.0f-x; }

        template <> bool Pixel<uint8_t>::IsZero(const uint8_t &C) { return C<=0; }
        template <> bool Pixel<float>  ::IsZero(const float   &C) { return fabs(C)<=0.0f; }


        template <> uint8_t Pixel<uint8_t>::Average(uint8_t *arr, const size_t num)
        {
            assert(num>0);

            float ave = 0.0f;
            for(size_t i=0;i<num;++i)
            {
                ave += float(arr[i]);
            }
            return Y_INK_F2B(ave/num);
        }

        template <> float Pixel<float>::Average(float *arr, const size_t num)
        {
            assert(num>0);
            float ave  = 0.0f;
            for(size_t i=0;i<num;++i)
            {
                ave += arr[i];
            }
            return ave/num;
        }

        template < > int Pixel<uint8_t>::Compare(const uint8_t &lhs, const uint8_t &rhs)
        {
            return int(lhs)-int(rhs);
        }

        template < > int Pixel<float>::Compare(const float &lhs, const float &rhs)
        {
            return comparison::increasing(lhs,rhs);
        }

        template <> float Pixel<float>::Blend(const float &fg, const float &bg, const uint8_t alpha)
        {
            return fg * Crux::u_float[alpha] + bg * Crux::u_float[0xff-alpha];
        }

        template <> uint8_t Pixel<uint8_t>:: Blend( const uint8_t &fg, const uint8_t &bg, const uint8_t alpha)
        {
            switch(alpha)
            {
                case 0:    return bg;
                case 0xff: return fg;
                default: break;
            }
            const unsigned u = unsigned(fg) * alpha + unsigned(bg) * (0xff-alpha);
            return u>>8;
        }

    }
}
