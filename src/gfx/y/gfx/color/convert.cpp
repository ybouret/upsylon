
#include "y/gfx/color/convert.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{
    namespace GFX
    {
        

        const Kernel::rgb<float> Yscale( 0.299f,  0.587f, 0.114f);
        const Kernel::rgb<float> Uscale(-0.147f, -0.289f, 0.436f);
        const Kernel::rgb<float> Vscale( 0.615f, -0.515f,-0.100f);

#if 0
        float Convert:: RGB2Y(const float r, const float g, const float b) throw()
        {
            return YScale.r * r + YScale.g*g + YScale.b * b;
        }

        float Convert:: RGB2U(const float r, const float g, const float b) throw()
        {
            return UScale.r * r + UScale.g*g + UScale.b * b;
        }

        float Convert:: RGB2V(const float r, const float g, const float b) throw()
        {
            return VScale.r * r + VScale.g*g + VScale.b * b;
        }


        float Convert:: RGB2Y(const uint8_t r, const uint8_t g, const uint8_t b) throw()
        {
            return ToByte( RGB2Y(UnitFloat[r],UnitFloat[g],UnitFloat[b]) );
        }

        float Convert:: RGB2U(const uint8_t r, const uint8_t g, const uint8_t b) throw()
        {
            return ToByte( RGB2U(UnitFloat[r],UnitFloat[g],UnitFloat[b]) );
        }

        float Convert:: RGB2V(const uint8_t r, const uint8_t g, const uint8_t b) throw()
        {
            return ToByte( RGB2V(UnitFloat[r],UnitFloat[g],UnitFloat[b]) );
        }

        float Convert:: YUV2R( const YUV &_ ) throw()
        {
            return clamp<float>(0.0f, _.y + 1.140f * _.v,1.0f);
        }

        float Convert:: YUV2G( const YUV &_ ) throw()
        {
            return clamp<float>(0.0f, _.y - 0.394f * _.u - 0.581f * _.v,1.0f);
        }

        float Convert:: YUV2B( const YUV &_ ) throw()
        {
            return clamp<float>(0.0f, _.y + 2.028f * _.u,1.0f);
        }
#endif

        
        template <>
        rgbF Convert:: YUV2RGB<float>( const YUV &yuv ) throw()
        {
            return rgbF( YUV2R(yuv), YUV2G(yuv), YUV2B(yuv) );
        }

    }

}


