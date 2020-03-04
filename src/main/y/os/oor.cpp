#include "y/os/oor.hpp"
#include <cstring>

namespace upsylon {

    void out_of_reach:: copy(void *target, const void *source, const size_t bytes) throw()
    {
        assert( !(NULL==target&&bytes>0) );
        assert( !(NULL==source&&bytes>0) );
        memcpy(target,source,bytes);
    }

    void out_of_reach:: move(void *target, const void *source, const size_t bytes) throw()
    {
        assert( !(NULL==target&&bytes>0) );
        assert( !(NULL==source&&bytes>0) );
        memmove(target,source,bytes);
    }

    void  out_of_reach:: fill(void *target, const uint8_t byte, const size_t bytes) throw()
    {
        assert( !(NULL==target&&bytes>0) );
        memset(target,byte,bytes);
    }

    void out_of_reach:: fill_indices(size_t      *indices,
                                     const size_t length,
                                     const size_t offset) throw()
    {
        assert( !(NULL==indices&&length>0) );
        for(size_t i=0,j=offset;i<length;++i,++j)
        {
            indices[i] = j;
        }
    }
}
