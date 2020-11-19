#include "y/associative/hash/handle.hpp"
#include "y/object.hpp"

namespace upsylon
{
    hash_handle * hash_handle:: acquire( )
    {
        return object::acquire1<hash_handle>();
    }

    void hash_handle:: release(hash_handle *handle) throw()
    {
        assert(handle);
        object::release1(handle);
    }


}
