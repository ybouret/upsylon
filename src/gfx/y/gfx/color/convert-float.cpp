#include "y/gfx/color/convert.hpp"

namespace upsylon
{
    namespace GFX
    {
        
        template <>
        uint8_t Convert<uint8_t,float>:: From(const float &f) throw()
        {
            return Conv::ToByte(f);
        }
        
        template <>
        rgb Convert<rgb,float>::From(const float &f) throw()
        {
            const uint8_t u = Conv::ToByte(f);
            return rgb(u,u,u);
        }
        
        template <>
        rgba Convert<rgba,float>:: From(const float &f) throw()
        {
            const uint8_t u = Conv::ToByte(f);
            return rgba(u,u,u);
        }
        
        template <>
        YUV Convert<YUV,float>:: From(const float &f) throw()
        {
             return YUV(f,0,0);
        }
        
        
    }
    
}


