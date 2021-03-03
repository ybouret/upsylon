

#include "y/graphic/color/convert.hpp"

namespace upsylon
{

    namespace graphic
    {

        template <> float convert<float,YUV>:: from(const YUV &z) throw()
        {
            return z.y;
        }

        template <> uint8_t convert<uint8_t,YUV>:: from(const YUV &z) throw()
        {
            return crux::convert::to_byte(z.y);
        }

        template <>     rgbF convert<rgbF,YUV>:: from(const YUV &z) throw()
        {
            const float r =  z.y + 1.140f * z.v;
            const float g =  z.y - 0.394f * z.u - 0.581f * z.v;
            const float b =  z.y + 2.028f * z.u;
            return rgbF(r,g,b);
        }

        template <>     rgb convert<rgb,YUV>:: from(const YUV &z) throw()
        {
            const rgbF c = convert<rgbF,YUV>::from(z);
            return rgb(crux::convert::to_byte(c.r),
                       crux::convert::to_byte(c.g),
                       crux::convert::to_byte(c.b) );
        }

        template <>     rgba convert<rgba,YUV>:: from(const YUV &z) throw()
        {
            return convert<rgb,YUV>::from(z);
        }

    }


}


