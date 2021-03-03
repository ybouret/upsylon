
#include "y/graphic/color/convert.hpp"

namespace upsylon
{

    namespace graphic
    {
        template <>
        uint8_t convert<uint8_t,rgb>:: from(const rgb &c) throw()
        {
            return crux::convert::greyscale[ unsigned(c.r) + unsigned(c.g) + unsigned(c.b) ];
        }

        template <>
        float convert<float,rgb>:: from(const rgb &c) throw()
        {
            return crux::convert::unit_float[ convert<uint8_t,rgb>::from(c) ];
        }

        template <>
        rgba convert<rgba,rgb>:: from(const rgb &c) throw()
        {
            return c;
        }


        template <>
        YUV convert<YUV,rgb>:: from(const rgb &c) throw()
        {
            const float r = crux::convert::unit_float[c.r];
            const float g = crux::convert::unit_float[c.g];
            const float b = crux::convert::unit_float[c.b];
            return YUV( 0.299f*r+0.587f*g+0.114f*b,
                       -0.147f*r-0.289f*g+0.436f*b,
                       0.615f*r-0.515f*g-0.100f*b);
        }


    }
}


