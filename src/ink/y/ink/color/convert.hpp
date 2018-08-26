//! \file
#ifndef Y_INK_COLOR_CONVERT_INCLUDED
#define Y_INK_COLOR_CONVERT_INCLUDED 1

#include "y/ink/color/yuv.hpp"

namespace upsylon
{
    namespace ink
    {
        struct color_convert
        {
            template <typename TGT,typename SRC> static
            TGT make(const SRC &src);
        };

        template <>
        float color_convert::make<float,RGB>(const RGB &C)
        {
            return crux::grey_scale_f(C.r,C.g,C.b);
        }

        template <>
        uint8_t color_convert::make<uint8_t,RGB>(const RGB &C)
        {
            return crux::grey_scale_u(C.r,C.g,C.b);
        }

        template <>
        float color_convert::make<float,RGBA>(const RGBA &C)
        {
            return crux::grey_scale_f(C.r,C.g,C.b);
        }

        template <>
        uint8_t color_convert::make<uint8_t,RGBA>(const RGBA &C)
        {
            return crux::grey_scale_u(C.r,C.g,C.b);
        }

    }
}

#endif

