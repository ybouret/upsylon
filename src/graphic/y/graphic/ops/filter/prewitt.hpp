//! \file

#ifndef Y_GRAPHIC_PREWITT_INCLUDED
#define Y_GRAPHIC_PREWITT_INCLUDED 1

#include "y/graphic/ops/filter/gradients.hpp"

namespace upsylon {

    namespace Graphic {


        struct Prewitt
        {
            typedef Filter<int8_t> FilterType;

            Y_GRAPHIC_FILTER_DECL(X3);
            Y_GRAPHIC_FILTER_DECL(Y3);
            Y_GRAPHIC_FILTER_DECL(X5);
            Y_GRAPHIC_FILTER_DECL(Y5);

            Y_GRAPHICS_GRADIENTS_DECL(3);
            Y_GRAPHICS_GRADIENTS_DECL(5);

        };

    }

}

#endif

