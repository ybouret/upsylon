
//! \file

#ifndef Y_MEMORY_STORAGE_INCLUDED
#define Y_MEMORY_STORAGE_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    namespace memory {

        //______________________________________________________________________
        //
        //
        //! memory storage class
        //
        //______________________________________________________________________
        struct storage {

            //! possible model
            enum model {
                unused, //!< no memory
                shared, //!< using small_object
                pooled, //!< using memory::pooled
                global  //!< using memory::global
            };

            //! return human readable text for storage
            static const char *text(const model which) throw();

        };


    }

}

#endif

