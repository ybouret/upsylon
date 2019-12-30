//! \file

#ifndef Y_GRAPHIC_SCHARR_INCLUDED
#define Y_GRAPHIC_SCHARR_INCLUDED 1

#include "y/graphic/ops/filter/gradients.hpp"

namespace upsylon {

    namespace Graphic {

        //! Scharr filters
        struct Scharr
        {
            typedef Filter<int8_t> FilterType; //!< alias

            Y_GRAPHIC_FILTER_DECL(X3); //!< X3
            Y_GRAPHIC_FILTER_DECL(Y3); //!< Y3
            Y_GRAPHIC_FILTER_DECL(X5); //!< X5
            Y_GRAPHIC_FILTER_DECL(Y5); //!< Y5

            Y_GRAPHICS_GRADIENTS_DECL(3); //!< X3/Y3
            Y_GRAPHICS_GRADIENTS_DECL(5); //!< X5/Y5
        };

    }

}

#endif

