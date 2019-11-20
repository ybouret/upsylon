
//! \file

#ifndef Y_GRAPHIC_TYPES_INCLUDED
#define Y_GRAPHIC_TYPES_INCLUDED 1

#include "y/ptr/counted.hpp"
#include "y/type/point2d.hpp"

namespace upsylon {

    namespace Graphic {

        typedef counted_object  Object; //!< base type for dynamic object
        typedef point2d<unit_t> Point;  //!< logical point
    }
}

#endif


