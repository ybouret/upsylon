
#include "y/gfx/color/random.hpp"
#include "y/gfx/color/rgb.hpp"
#include "y/gfx/color/yuv.hpp"
namespace upsylon
{
    
    namespace graphic
    {
        template <>
        uint8_t random_color:: get<uint8_t>( randomized::bits &ran ) throw()
        {
            return ran.full<uint8_t>();
        }
        
        template <>
        float random_color:: get<float>( randomized::bits &ran ) throw()
        {
            return ran.to<float>();
        }
        
        template <>
        double random_color:: get<double>( randomized::bits &ran ) throw()
        {
            return ran.to<double>();
        }
        
        template <>
        rgb random_color:: get<rgb>( randomized::bits &ran ) throw()
        {
            const uint8_t r = ran.full<uint8_t>();
            const uint8_t g = ran.full<uint8_t>();
            const uint8_t b = ran.full<uint8_t>();
            return rgb(r,g,b);
        }
        
        template <>
        rgba random_color:: get<rgba>( randomized::bits &ran ) throw()
        {
            const uint8_t r = ran.full<uint8_t>();
            const uint8_t g = ran.full<uint8_t>();
            const uint8_t b = ran.full<uint8_t>();
            const uint8_t a = ran.full<uint8_t>();
            return rgba(r,g,b,a);
        }
        
        template <>
        YUV random_color:: get<YUV>( randomized::bits &ran ) throw()
        {
            const float y = ran.to<float>();
            const float u = YUV::Umin + ran.to<float>() * (YUV::Umax-YUV::Umin);
            const float v = YUV::Vmin + ran.to<float>() * (YUV::Vmax-YUV::Vmin);
            return YUV(y,u,v);
        }
    }
    
}
