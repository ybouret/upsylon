#include "y/color/convert.hpp"

namespace upsylon
{

        template <> uint8_t convert_color<uint8_t,float>:: from(const float &f) throw()
        {
            return core::convert_color::to_byte(f);
        }
        
        template <>         rgb convert_color<rgb,float>:: from(const float &f) throw()
        {
            const uint8_t u = core::convert_color::to_byte(f);
            return rgb(u,u,u);
        }
        
        template <>
        rgba convert_color<rgba,float>:: from(const float &f) throw()
        {
            const uint8_t u = core::convert_color::to_byte(f);
            return rgba(u,u,u);
        }
        
        template <>
        YUV convert_color<YUV,float>:: from(const float &f) throw()
        {
             return YUV(f,0,0);
        }
        

}


