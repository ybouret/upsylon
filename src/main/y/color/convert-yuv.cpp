

#include "y/color/convert.hpp"

namespace upsylon
{

    template <> float convert_color<float,YUV>:: from(const YUV &z) throw()
    {
        return z.y;
    }

    template <> uint8_t convert_color<uint8_t,YUV>:: from(const YUV &z) throw()
    {
        return core::convert_color::to_byte(z.y);
    }

    template <>     rgbF convert_color<rgbF,YUV>:: from(const YUV &z) throw()
    {
        const float r =  z.y + 1.140f * z.v;
        const float g =  z.y - 0.394f * z.u - 0.581f * z.v;
        const float b =  z.y + 2.028f * z.u;
        return rgbF(r,g,b);
    }

    template <>     rgb convert_color<rgb,YUV>:: from(const YUV &z) throw()
    {
        const rgbF c = convert_color<rgbF,YUV>::from(z);
        return rgb(core::convert_color::to_byte(c.r),
                   core::convert_color::to_byte(c.g),
                   core::convert_color::to_byte(c.b) );
    }

    template <>     rgba convert_color<rgba,YUV>:: from(const YUV &z) throw()
    {
        return convert_color<rgb,YUV>::from(z);
    }



}


