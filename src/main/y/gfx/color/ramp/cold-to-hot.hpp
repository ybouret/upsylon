
//! \file

#ifndef Y_GFX_COLOR_RAMP_CTH_INCLUDED
#define Y_GFX_COLOR_RAMP_CTH_INCLUDED 1

#include "y/gfx/color/ramp/tableau.hpp"

namespace upsylon
{

    namespace graphic
    {
        //______________________________________________________________________
        //
        //
        //! create a cold to hot repository
        //
        //______________________________________________________________________
        struct cold_to_hot
        {
            static const size_t size;    //!< blue->red
            static const rgba   data[];  //!< blue->red
        };


    }

}

#endif
