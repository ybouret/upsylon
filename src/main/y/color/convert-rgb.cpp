
#include "y/color/convert.hpp"

namespace upsylon
{

    template <>
    uint8_t convert_color<uint8_t,rgb>:: from(const rgb &c) throw()
    {
        return core::convert_color::greyscale[ unsigned(c.r) + unsigned(c.g) + unsigned(c.b) ];
    }

    template <>
    float convert_color<float,rgb>:: from(const rgb &c) throw()
    {
        return core::convert_color::unit_float[ convert_color<uint8_t,rgb>::from(c) ];
    }

    template <>
    rgba convert_color<rgba,rgb>:: from(const rgb &c) throw()
    {
        return c;
    }


    template <>
    YUV convert_color<YUV,rgb>:: from(const rgb &c) throw()
    {
        const float r = core::convert_color::unit_float[c.r];
        const float g = core::convert_color::unit_float[c.g];
        const float b = core::convert_color::unit_float[c.b];
        return YUV( 0.299f*r+0.587f*g+0.114f*b,
                   -0.147f*r-0.289f*g+0.436f*b,
                   0.615f*r-0.515f*g-0.100f*b);
    }


    
}


