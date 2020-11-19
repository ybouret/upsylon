//! \file

#ifndef Y_HASH_HANDLE_INCLUDED
#define Y_HASH_HANDLE_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{
    struct hash_handle
    {
        hash_handle *next;
        hash_handle *prev;
        size_t       hkey;
        void        *node;
        static hash_handle *acquire();
        static void         release(hash_handle *) throw();
    };
}

#endif
