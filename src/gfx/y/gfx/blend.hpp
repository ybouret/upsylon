//! \file


#ifndef Y_GFX_BLEND_INCLUDED
#define Y_GFX_BLEND_INCLUDED 1

#include "y/gfx/color/rgb.hpp"
#include <cmath>

namespace upsylon
{
    namespace graphic
    {

        template <typename ALPHA, typename T>
        struct blend
        {
            static T mix(const ALPHA alpha, const T fg, const T bg) throw();
        };


        template <>
        struct blend<float,float>
        {
            static inline float mix(const float alpha, const float fg, const float bg) throw()
            {
                return bg + alpha*(fg-bg);
            }
        };


        struct blend_
        {

            static const float * const fshift; //!< float[-255..255]
            static const int   * const ishift; //!< float[-255..255]

            static inline
            uint8_t mix1(const uint8_t alpha,
                         const uint8_t fg,
                         const uint8_t bg) throw()
            {
                switch(alpha)
                {
                    case 0x00: return bg;
                    case 0xff: return fg;
                    case 0x80: return uint8_t((unsigned(fg)+unsigned(bg))>>1);
                    default:
                        break;
                }
                const int FG = int ( unsigned(fg) << 8 );
                const int BG = int(alpha) * ishift[ int(fg) - int(bg) ];
                return unsigned(FG+BG)>>8;
            }

            static inline
            uint8_t mixf(const float   alpha,
                         const uint8_t fg,
                         const uint8_t bg) throw()
            {
                const float resf = fshift[bg] + alpha * fshift[ int(fg) - int(bg) ];
                return uint8_t( floorf(resf+0.5f) );
            }

            //! mix only rgb parts of LHS/RHS
            template <typename LHS, typename RHS> static inline
            rgb mix(const float w, const LHS lhs, const RHS rhs) throw()
            {
                return rgb(mixf(w,lhs.r,rhs.r),
                           mixf(w,lhs.g,rhs.g),
                           mixf(w,lhs.b,rhs.b));
            }




        };


    }

}

#endif

