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
        
        //______________________________________________________________________
        //
        //
        //! managing named colors
        //
        //______________________________________________________________________
        struct named_color
        {
            static const rgba table[Y_GFX_NAMED_COLORS];        //!< table or rgba value
            static rgba       rand(randomized::bits &) throw(); //!< a random, non-zero color
            static rgba       get(const size_t tag) throw();    //!< non linear color attribution
        };

    }
}

#endif
