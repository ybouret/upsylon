//! \file

#ifndef Y_GRAPHIC_PREWITT_INCLUDED
#define Y_GRAPHIC_PREWITT_INCLUDED 1

#include "y/graphic/ops/filter.hpp"

namespace upsylon {

    namespace Graphic {


        struct Prewitt
        {
            typedef Filter<uint8_t> FilterType;

            Y_GRAPHIC_FILTER_DECL(X3);
            Y_GRAPHIC_FILTER_DECL(Y3);
            Y_GRAPHIC_FILTER_DECL(X5);
            Y_GRAPHIC_FILTER_DECL(Y5);
        };

    }

}

#endif

