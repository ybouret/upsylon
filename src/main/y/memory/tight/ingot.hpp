//! \file

#ifndef Y_MEMORY_TIGHT_INGOT_INCLUDED
#define Y_MEMORY_TIGHT_INGOT_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    namespace memory {

        namespace tight {

            //__________________________________________________________________
            //
            //
            //! binary layout for an ingot
            //
            //__________________________________________________________________
            struct ingot
            {
                ingot *next; //!< for list/pool
                ingot *prev; //!< for list
            };

        }
    }
}

#endif
