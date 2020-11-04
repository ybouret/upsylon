
#include "y/associative/hash/meta-pool.hpp"
#include "y/sort/merge.hpp"

namespace upsylon
{
    hash_meta_pool:: hash_meta_pool() throw() : hash_meta_list()
    {

    }

    hash_meta_pool:: ~hash_meta_pool() throw()
    {
        while(size) hash_meta_node::release( pop_back() );
    }

    hash_meta_node * hash_meta_pool:: query(const size_t hkey, void *addr)
    {
        assert(NULL!=addr);
        hash_meta_node *node = size ? pop_front() : hash_meta_node::acquire();
        node->hkey = hkey;
        node->addr = addr;
        return node;
    }

    void hash_meta_pool:: store(hash_meta_node *node) throw()
    {
        assert(node);
        assert(NULL==node->next);
        assert(NULL==node->prev);
        push_front(node);
    }

    void hash_meta_pool:: sort() throw()
    {
        merging<hash_meta_node>::sort_by_increasing_address(*this);
    }


}
