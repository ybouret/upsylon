//! \file

#ifndef Y_GFX_TYPES_INCLUDED
#define Y_GFX_TYPES_INCLUDED 1

#include "y/type/point2d.hpp"
#include "y/core/zero-flux-index.hpp"
#include "y/strfwd.hpp"

namespace upsylon
{

    namespace graphic
    {

        typedef point2d<unit_t>          coord; //!< logical coordinate
        typedef core::zero_flux_index<0> zflux; //!< zero-flux computation

        //! helper to check setups
        struct checking
        {
            static unit_t     geqz(const unit_t value, const char *name); //!< value>=0
            static unit_t     gtz(const unit_t value, const char *name);  //!< value>0
            static const char width[];  //!< "width"
            static const char height[]; //!< "height"
            static const char depth[];  //!< "depth"
        };
        
        //! parsing helpers
        struct parsing
        {
            static coord wxh(const string &); //!< 'wxh'
            static coord wxh(const char   *); //!< 'wxh', wrapper

        };
        
    }

}


#endif

