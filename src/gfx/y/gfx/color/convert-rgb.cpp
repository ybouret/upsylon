
#include "y/gfx/color/convert.hpp"

namespace upsylon
{
    namespace GFX
    {
        
        template <>
        uint8_t Convert<uint8_t,rgb>:: From(const rgb &c) throw()
        {
            return Conv::Table[ unsigned(c.r) + unsigned(c.g) + unsigned(c.b) ];
        }
        
        template <>
        float Convert<float,rgb>::From(const rgb &c) throw()
        {
            return Conv::UnitFloat[ Convert<uint8_t,rgb>::From(c) ];
        }
        
        template <>
        rgba Convert<rgba,rgb>:: From(const rgb &c) throw()
        {
            return c;
        }
        
        
        template <>
        YUV Convert<YUV,rgb>:: From(const rgb &c) throw()
        {
            const float r = Conv::UnitFloat[c.r];
            const float g = Conv::UnitFloat[c.g];
            const float b = Conv::UnitFloat[c.b];
            return YUV( 0.299f*r+0.587f*g+0.114f*b,
                       -0.147f*r-0.289f*g+0.436f*b,
                       0.615f*r-0.515f*g-0.100f*b);
        }
        
    }
    
}


