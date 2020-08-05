
//! \file

#ifndef Y_MEMORY_SMALL_STONE_INCLUDED
#define Y_MEMORY_SMALL_STONE_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    namespace memory {

        namespace small {


            struct stone
            {
                stone *next;
                stone *prev;
            };

        }
    }
}

#endif
