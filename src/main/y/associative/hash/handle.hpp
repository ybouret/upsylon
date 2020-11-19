//! \file

#ifndef Y_HASH_HANDLE_INCLUDED
#define Y_HASH_HANDLE_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{
    //__________________________________________________________________________
    //
    //
    //! lightweight handle to any node
    //
    //__________________________________________________________________________
    struct hash_handle
    {
        hash_handle *next; //!< for list
        hash_handle *prev; //!< for list
        size_t       hkey; //!< hashed key
        void        *node; //!< address of handled node

        static hash_handle *acquire();                       //!< by object
        static void         release(hash_handle *) throw();  //!< by object
    };
}

#endif
