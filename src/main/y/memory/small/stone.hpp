
//! \file

#ifndef Y_MEMORY_SMALL_STONE_INCLUDED
#define Y_MEMORY_SMALL_STONE_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    namespace memory {

        namespace small {

            //! binary layout for a stone
            struct stone
            {
                stone *next; //!< for list/pool
                stone *prev; //!< for list
            };

        }
    }
}

#endif
