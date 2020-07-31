#include "y/memory/allocator/pooled.hpp"
#include "y/memory/carver.hpp"
#include "y/object.hpp"
#include "y/type/self-destruct.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/block/zset.hpp"

namespace upsylon
{
    namespace memory
    {

        const at_exit::longevity  pooled:: life_time =  longevity_for::memory_pooled;
        
        namespace
        {
            static uint64_t        ___carver[ Y_U64_FOR_ITEM(carver) ] = { 0 };
        }


        pooled:: ~pooled() throw()
        {
            self_destruct( *aliasing::as<carver>(___carver) );
            Y_BZSET_STATIC(___carver);
        }

        pooled:: pooled() throw()
        {
            Y_BZSET_STATIC(___carver);
            new ( aliasing::anonymous(___carver) ) carver( Y_CHUNK_SIZE );
        }

        void * pooled:: acquire( size_t &n )
        {
            static allocator &mgr = *aliasing::as<carver>(___carver);
            Y_LOCK(access);
            return mgr.acquire(n);
        }

        void pooled:: release(void * &p, size_t &n) throw()
        {
            static allocator &mgr =  *aliasing::as<carver>(___carver);
            Y_LOCK(access);
            mgr.release(p,n);
        }

        bool  pooled:: compact(void * &addr, size_t &capa, const size_t size ) throw()
        {
            static carver &crv = *aliasing::as<carver>(___carver);
            Y_LOCK(access);
            return crv.compact(addr,capa,size);
        }


    }
}


