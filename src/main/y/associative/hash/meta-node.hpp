

//! \file
#ifndef Y_HASHED_META_NODE_INCLUDED
#define Y_HASHED_META_NODE_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{

    class hash_meta_node
    {
    public:
        hash_meta_node *next;
        hash_meta_node *prev;
        size_t          hkey;
        void           *addr;

        static hash_meta_node *acquire();
        static void            release(hash_meta_node *) throw();

    private:
        hash_meta_node() throw();
        ~hash_meta_node() throw();
        Y_DISABLE_COPY_AND_ASSIGN(hash_meta_node);
    };



}

#endif
