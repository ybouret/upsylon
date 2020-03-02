//! \file

#ifndef Y_OS_OOR_INCLUDED
#define Y_OS_OOR_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    //! out of reach wrappers
    struct out_of_reach
    {
        static void copy(void *target, const void *source, const size_t bytes) throw(); //!< memcpy
        static void move(void *target, const void *source, const size_t bytes) throw(); //!< memmove
    };


}

#endif
