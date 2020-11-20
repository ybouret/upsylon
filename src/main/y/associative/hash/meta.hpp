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
    struct hash_meta
    {
        hash_meta *next; //!< for list
        hash_meta *prev; //!< for list
        size_t     hkey; //!< hashed key
        void      *node; //!< address of handled node

        static hash_meta *acquire();                     //!< by object
        static void       release(hash_meta *) throw();  //!< by object
    };
}

#endif
