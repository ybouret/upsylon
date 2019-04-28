#include "y/memory/pooled.hpp"
#include "y/memory/carver.hpp"
#include "y/memory/io.hpp"
#include "y/object.hpp"

namespace upsylon
{
    namespace memory
    {

        namespace
        {
            static uint64_t        ___carver[ Y_U64_FOR_ITEM(carver) ] = { 0 };
            static inline void     __zcarver() throw()
            {
                for(size_t i=0;i< sizeof(___carver)/sizeof(___carver[0]);++i) ___carver[i] = 0;
            }

            static inline carver * __carver() throw()
            {
                return io::__force<carver>(___carver);
            }
        }


        pooled:: ~pooled() throw()
        {
            destruct(__carver());
            __zcarver();
        }

        pooled:: pooled() throw()
        {
            __zcarver();
            new ( __carver() ) carver( Y_CHUNK_SIZE );
        }

        void * pooled:: acquire( size_t &n )
        {
            static allocator &mgr = * __carver();
            Y_LOCK(access);
            return mgr.acquire(n);
        }

        void pooled:: release(void * &p, size_t &n) throw()
        {
            static allocator &mgr = * __carver();
            Y_LOCK(access);
            mgr.release(p,n);
        }

    }
}


