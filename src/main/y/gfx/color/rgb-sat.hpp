
//! \file

#ifndef Y_COLOR_RGB_SAT_INCLUDED
#define Y_COLOR_RGB_SAT_INCLUDED 1

#include "y/gfx/color/rgb.hpp"

namespace upsylon
{
    namespace graphic
    {
        

        //! saturated function
        struct saturated
        {
            static rgb  of(const rgb  &) throw(); //!< for rgb
            static rgba of(const rgba &) throw(); //!< for rgba
        };

    }

}

#endif
