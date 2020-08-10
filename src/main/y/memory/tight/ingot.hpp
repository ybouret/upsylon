
//! \file

#ifndef Y_MEMORY_TIGHT_STONE_INCLUDED
#define Y_MEMORY_TIGHT_STONE_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    namespace memory {

        namespace small {

            //! binary layout for an ingot
            struct ingot
            {
                ingot *next; //!< for list/pool
                ingot *prev; //!< for list
            };

        }
    }
}

#endif
