//! \file

#ifndef Y_GRAPHIC_SOBEL_INCLUDED
#define Y_GRAPHIC_SOBEL_INCLUDED 1

#include "y/graphic/ops/filter.hpp"

namespace upsylon {

    namespace Graphic {


        struct Sobel
        {
            typedef Filter<int8_t> FilterType;

            Y_GRAPHIC_FILTER_DECL(X3);
            Y_GRAPHIC_FILTER_DECL(Y3);
            Y_GRAPHIC_FILTER_DECL(X5);
            Y_GRAPHIC_FILTER_DECL(Y5);

        };

    }

}

#endif

