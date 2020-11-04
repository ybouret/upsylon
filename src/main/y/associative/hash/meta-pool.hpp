
//! \file
#ifndef Y_HASHED_META_POOL_INCLUDED
#define Y_HASHED_META_POOL_INCLUDED 1

#include "y/associative/hash/meta-node.hpp"
#include "y/core/list.hpp"

namespace upsylon
{
    typedef core::list_of<hash_meta_node> hash_meta_list;


    class hash_meta_pool : public hash_meta_list
    {
    public:
        explicit hash_meta_pool() throw();
        virtual ~hash_meta_pool() throw();

        hash_meta_node *query(const size_t hkey, void *addr);
        void            store(hash_meta_node *node) throw();

        void sort() throw();

    private:
        Y_DISABLE_COPY_AND_ASSIGN(hash_meta_pool);

    };


}

#endif
