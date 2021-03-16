//! \file

#ifndef Y_GFX_TYPES_INCLUDED
#define Y_GFX_TYPES_INCLUDED 1

#include "y/type/point2d.hpp"
#include "y/core/zero-flux-index.hpp"
#include "y/strfwd.hpp"
#include "y/core/knode.hpp"

namespace upsylon
{

    namespace graphic
    {

        //______________________________________________________________________
        //
        //
        // global types
        //
        //______________________________________________________________________
        typedef point2d<unit_t>          coord; //!< logical coordinate
        typedef core::zero_flux_index<0> zflux; //!< zero-flux computation
        
        typedef core::knode<coord> cNode; //!< coordinate node
        typedef cNode::list_type   cList; //!< coordinate list
        typedef cNode::pool_type   cPool; //!< coordinate pool
        
        //______________________________________________________________________
        //
        //
        //! helper to check setups
        //
        //______________________________________________________________________
        struct checking
        {
            static unit_t     geqz(const unit_t value, const char *name); //!< value>=0
            static unit_t     gtz(const unit_t value, const char *name);  //!< value>0
            static const char width[];  //!< "width"
            static const char height[]; //!< "height"
            static const char depth[];  //!< "depth"
        };

        //______________________________________________________________________
        //
        //
        //! parsing helpers
        //
        //______________________________________________________________________
        struct parsing
        {
            static coord wxh(const string &); //!< 'wxh'
            static coord wxh(const char   *); //!< 'wxh', wrapper
        };
        
    }

}


#endif

