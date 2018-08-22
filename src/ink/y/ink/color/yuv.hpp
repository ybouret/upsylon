//! \file
#ifndef Y_INK_COLOR_YUV_INCLUDED
#define Y_INK_COLOR_YUV_INCLUDED 1

#include "y/ink/types.hpp"
#include "y/ink/color/rgb.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{
    namespace ink
    {

        //! YUV format
        class YUV
        {
        public:
            float y; //!< luminance   Y
            float u; //!< chrominance U
            float v; //!< chrominance V
            //!  constructor
            inline  YUV(const float Y=0,const float U=0,const float V=0) throw() : y(Y), u(U), v(V) {}
            //! destructor
            inline ~YUV() throw() {}
            //! copy
            inline  YUV(const YUV &other) throw() : y(other.y), u(other.u), v(other.v) {}
            //! assign
            inline  YUV & operator=( const YUV &other ) throw()
            {
                y=other.y; u=other.u; v=other.v;
                return *this;
            }

            //! YUV->RGB
            static inline
            RGB toRGB(const YUV &C) throw()
            {
                const float rf = C.y                  + 1.13983f*C.v;
                const float gf = C.y - 0.39465f * C.u - 0.58060f*C.v;
                const float bf = C.y + 2.03211f * C.u;
                return RGB(Y_INK_F2B(clamp<float>(0,rf,1)),
                           Y_INK_F2B(clamp<float>(0,gf,1)),
                           Y_INK_F2B(clamp<float>(0,bf,1))
                           );
            }

            //! RGB->YUV
            static inline
            YUV fromRGB(const RGB &C) throw()
            {
                const float rf = crux::u_float[C.r];
                const float gf = crux::u_float[C.g];
                const float bf = crux::u_float[C.b];
                const float Y  =  0.29900f*rf+0.58700f*gf+0.11400f*bf;
                const float U  = -0.14713f*rf-0.28886f*gf+0.43600f*bf;
                const float V  =  0.61500f*rf-0.51499f*gf-0.10001f*bf;
                return YUV(Y,U,V);
            }

            //! RGBA->YUV
            static inline
            YUV fromRGBA(const RGBA &C) throw()
            {
                const RGB tmp(C.r,C.g,C.b);
                return fromRGB(tmp);
            }

        };
    }
}

#if !defined(Y_INK_PIXEL_IMPL)
Y_PIXEL_DECL(upsylon::ink::YUV); //!< extern template for YUV pixel
#endif


#endif
