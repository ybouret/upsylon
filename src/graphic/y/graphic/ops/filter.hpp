//! \file

#ifndef Y_GRAPHIC_OPS_FILTER_INCLUDED
#define Y_GRAPHIC_OPS_FILTER_INCLUDED 1

#include "y/graphic/pixmap.hpp"
#include "y/oxide/field2d.hpp"

namespace upsylon {

    namespace Graphic {

        template <typename T>
        struct Filter
        {
            typedef Oxide::Field2D<T> Type;
            
        };

    }
}


#endif

