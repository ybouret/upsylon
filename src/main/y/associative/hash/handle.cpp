#include "y/associative/hash/handle.hpp"
#include "y/object.hpp"

namespace upsylon
{
    hash_handle * hash_handle:: acquire(const size_t h,void *p)
    {
        hash_handle *handle = object::acquire1<hash_handle>();
        handle->hkey = h;
        handle->node = p;
        return 0;
    }

    void hash_handle:: release(hash_handle *handle) throw()
    {
        assert(handle);
        object::release1(handle);
    }


}
