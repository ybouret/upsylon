

#include "y/gfx/color/convert.hpp"

namespace upsylon
{
    namespace GFX
    {

        template <>
        float Convert<float,YUV>:: From(const YUV &z) throw()
        {
            return z.y;
        }

        template <>
        uint8_t Convert<uint8_t,YUV>:: From(const YUV &z) throw()
        {
            return Conv::ToByte(z.y);
        }

        template <>
        rgbF Convert<rgbF,YUV>:: From(const YUV &z) throw()
        {
            const float r =  z.y + 1.140f * z.v;
            const float g =  z.y - 0.394f * z.u - 0.581f * z.v;
            const float b =  z.y + 2.028f * z.u;
            return rgbF(r,g,b);
        }

        template <>
        rgb Convert<rgb,YUV>:: From(const YUV &z) throw()
        {
            const rgbF c = Convert<rgbF,YUV>::From(z);
            return rgb( Conv::ToByte(c.r), Conv::ToByte(c.g), Conv::ToByte(c.b) );
        }

        template <>
        rgba Convert<rgba,YUV>:: From(const YUV &z) throw()
        {
            return Convert<rgb,YUV>::From(z);
        }


    }

}


