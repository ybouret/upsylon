

//! \file
#ifndef Y_HASHED_META_NODE_INCLUDED
#define Y_HASHED_META_NODE_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{

    //! meta node
    class hash_meta_node
    {
    public:
        hash_meta_node *next; //!< for list
        hash_meta_node *prev; //!< for list
        size_t          hkey; //!< code
        void           *addr; //!< data address

        static hash_meta_node *acquire(); //!< create
        static void            release(hash_meta_node *) throw(); //!< release

    private:
        hash_meta_node() throw();
        ~hash_meta_node() throw();
        Y_DISABLE_COPY_AND_ASSIGN(hash_meta_node);
    };



}

#endif
