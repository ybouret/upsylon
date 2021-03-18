//! \file

#ifndef Y_GFX_FILTERS_PREWITT_INCLUDED
#define Y_GFX_FILTERS_PREWITT_INCLUDED 1

#include "y/gfx/filters.hpp"

namespace upsylon
{
    
    namespace graphic
    {
        
        Y_FILTERS_DECL(Prewitt,3); //!< Prewitt 3x3
        Y_FILTERS_DECL(Prewitt,5); //!< Prewitt 5x5
        Y_FILTERS_DECL(Prewitt,7); //!< Prewitt 7x7

    }
    
}

#endif
