//! \file

#ifndef Y_TYPE_CLASS_CAST_INCLUDED
#define Y_TYPE_CLASS_CAST_INCLUDED 1

#include "y/type/class-conversion.hpp"

namespace upsylon {

    template <typename T,typename U>
    struct class_cast
    {
        static const bool derived = Y_IS_SUPERSUBCLASS(U,T);

    };

}


#endif
