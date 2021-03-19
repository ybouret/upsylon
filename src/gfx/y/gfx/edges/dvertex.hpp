//! \file

#ifndef Y_GFX_EDGES_DVERTEX_INCLUDED
#define Y_GFX_EDGES_DVERTEX_INCLUDED 1

#include "y/gfx/types.hpp"

namespace upsylon
{
    namespace graphic
    {

        namespace edges
        {

            //__________________________________________________________________
            //
            //
            //! digitized vertex definition
            //
            //__________________________________________________________________
            struct dvertex
            {
                typedef point2d<int8_t> type; //!< in [-1:1]^2

                //! return index
                /**
                 - [0:0] -> 0
                 */
                static size_t index_of(const type &vtx) throw();
            };

        }

    }

}

#endif
