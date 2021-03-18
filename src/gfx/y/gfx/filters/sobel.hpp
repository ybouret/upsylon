//! \file

#ifndef Y_GFX_FILTERS_SOBEL_INCLUDED
#define Y_GFX_FILTERS_SOBEL_INCLUDED 1

#include "y/gfx/filters.hpp"

namespace upsylon
{
    
    namespace graphic
    {
        
        Y_FILTERS_DECL(Sobel,3); //!< Sobel 3x3
        Y_FILTERS_DECL(Sobel,5); //!< Sobel 5x5
        Y_FILTERS_DECL(Sobel,7); //!< Sobel 7x7

    }
    
}

#endif
