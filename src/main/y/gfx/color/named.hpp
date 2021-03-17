//! \file

#ifndef Y_GFX_NAMED_COLOR_INCLUDED
#define Y_GFX_NAMED_COLOR_INCLUDED 1

#include "y/gfx/color/rgb.hpp"
#include "y/gfx/color/named-inc.hpp"

namespace upsylon
{
    namespace randomized
    {
        class bits;
    }

    namespace graphic
    {

        struct named_color
        {
            static const rgba table[Y_GFX_NAMED_COLORS];
            static rgba       rand(randomized::bits &) throw();
            static rgba       get(const size_t tag) throw();
        };

    }
}

#endif
