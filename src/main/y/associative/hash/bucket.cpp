
#include "y/associative/hash/bucket.hpp"
#include "y/sort/merge.hpp"

namespace upsylon
{
    hash_bucket:: hash_bucket() throw() : hash_handles()
    {
    }

    hash_bucket:: ~hash_bucket() throw()
    {
        release();
    }

    
    void hash_bucket:: release() throw()
    {
        while(size) pop();

    }

    void hash_bucket:: reorder() throw()
    {
        merging<hash_handle>::sort_by_increasing_address(*this);
    }

    void hash_bucket:: push()
    {
        push_back( hash_handle::acquire() );
    }

    void hash_bucket:: pop() throw()
    {
        hash_handle:: release( pop_back() );
    }

    void hash_bucket:: store(hash_handle *handle) throw()
    {
        assert(handle);
        push_front(handle);
    }



    hash_handle * hash_bucket:: query(const size_t hkey,
                                      void        *node) throw()
    {
        assert(size);
        hash_handle *handle = pop_front();
        handle->hkey = hkey;
        handle->node = node;
        return handle;
    }

}
