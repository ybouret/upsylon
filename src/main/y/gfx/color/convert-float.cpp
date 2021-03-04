#include "y/gfx/color/convert.hpp"

namespace upsylon
{

    namespace  graphic
    {
        template <> uint8_t convert<uint8_t,float>:: from(const float &f) throw()
        {
            return crux::convert::to_byte(f);
        }
        
        template <>         rgb convert<rgb,float>:: from(const float &f) throw()
        {
            const uint8_t u = crux::convert::to_byte(f);
            return rgb(u,u,u);
        }
        
        template <>         rgba convert<rgba,float>:: from(const float &f) throw()
        {
            const uint8_t u = crux::convert::to_byte(f);
            return rgba(u,u,u);
        }
        
        template <>         YUV convert<YUV,float>:: from(const float &f) throw()
        {
             return YUV(f,0,0);
        }

    }


}


