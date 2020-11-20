#include "y/associative/hash/meta.hpp"
#include "y/object.hpp"

namespace upsylon
{
    hash_meta * hash_meta:: acquire( )
    {
        return object::acquire1<hash_meta>();
    }

    void hash_meta:: release(hash_meta *handle) throw()
    {
        assert(handle);
        object::release1(handle);
    }


}
