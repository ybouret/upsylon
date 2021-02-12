#include "y/gfx/color/convert.hpp"

namespace upsylon
{
    namespace GFX
    {
        
        template <> template <>
        uint8_t Convert<float>:: To<uint8_t>(const float &f) throw()
        {
            return Conv::ToByte(f);
        }
        
        template <> template <>
        rgb Convert<float>:: To<rgb>(const float &f) throw()
        {
            const uint8_t u = Convert<float>::To<uint8_t>(f);
            return rgb(u,u,u);
        }
        
        template <> template <>
        rgba Convert<float>:: To<rgba>(const float &f) throw()
        {
            const uint8_t u = Convert<float>::To<uint8_t>(f);
            return rgba(u,u,u);
        }
        
        template <> template <>
        YUV Convert<float>:: To<YUV>(const float &f) throw()
        {
             return YUV(f,0,0);
        }
        
        
        
    }
    
}


