//! \file


#ifndef Y_GFX_BLEND_INCLUDED
#define Y_GFX_BLEND_INCLUDED 1

#include "y/gfx/color/rgb.hpp"

namespace upsylon
{
    namespace graphic
    {

        struct blend
        {

            static const float * const shift; //!< float[-255..255]

            static inline
            uint8_t mix(const float weight, const uint8_t fg, const uint8_t bg) throw()
            {
                const float resf = shift[bg] + weight * shift[ int(fg) - int(bg) ];
                return uint8_t( floorf(resf+0.5f) );
            }

        };


    }

}

#endif

