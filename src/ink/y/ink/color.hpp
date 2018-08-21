//! \file
#ifndef Y_INK_COLOR_INCLUDED
#define Y_INK_COLOR_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{
    namespace ink
    {

        //! data for color types
        template <typename T>
        struct color
        {
            static const T opaque; //!< the opaque value
        };

    }
}

#endif

