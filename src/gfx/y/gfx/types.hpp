
//! \file

#ifndef Y_GFX_TYPES_INCLUDED
#define Y_GFX_TYPES_INCLUDED 1

#include "y/type/point2d.hpp"
#include "y/core/zero-flux-index.hpp"

namespace upsylon
{

    namespace graphic
    {
        typedef point2d<unit_t>          coord;
        typedef core::zero_flux_index<0> zflux;
        
        struct checking
        {
            static unit_t     geqz(const unit_t value, const char *name);
            static const char width[];
            static const char height[];
        };
        
        
    }

}


#endif

