
#include "y/associative/hash/slot.hpp"
#include "y/sort/merge.hpp"

namespace upsylon
{
    hash_slot:: hash_slot() throw() : hash_metas()
    {
    }

    hash_slot:: ~hash_slot() throw()
    {
        release();
    }

    
    void hash_slot:: release() throw()
    {
        while(size) pop();

    }

    void hash_slot:: reorder() throw()
    {
        merging<hash_meta>::sort_by_increasing_address(*this);
    }

    void hash_slot:: push()
    {
        push_back( hash_meta::acquire() );
    }

    void hash_slot:: pop() throw()
    {
        hash_meta:: release( pop_back() );
    }

    void hash_slot:: store(hash_meta *handle) throw()
    {
        assert(handle);
        push_front(handle);
    }

    

    hash_meta * hash_slot:: query(const size_t hkey,
                                  void        *node) throw()
    {
        assert(size);
        hash_meta *handle = pop_front();
        handle->hkey = hkey;
        handle->node = node;
        return handle;
    }

}
