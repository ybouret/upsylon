//! \file


#ifndef Y_GFX_BLEND_INCLUDED
#define Y_GFX_BLEND_INCLUDED 1

#include "y/gfx/color/rgb.hpp"
#include <cmath>

namespace upsylon
{
    namespace graphic
    {

        struct blend
        {

            static const float * const shift; //!< float[-255..255]

            static inline
            uint8_t mixf(const float   weight,
                         const uint8_t fg,
                         const uint8_t bg) throw()
            {
                const float resf = shift[bg] + weight * shift[ int(fg) - int(bg) ];
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

