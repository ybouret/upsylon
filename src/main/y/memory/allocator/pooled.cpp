#include "y/memory/allocator/pooled.hpp"
#include "y/memory/joint/ward.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/self-destruct.hpp"
#include "y/object-parameters.hpp"

namespace upsylon
{
    namespace memory
    {


        const at_exit::longevity  pooled:: life_time =  limit_of<at_exit::longevity>::maximum - 1;

        namespace
        {
            static uint64_t impl[ Y_U64_FOR_ITEM(joint::ward) ] = { 0 };
        }


        pooled:: ~pooled() throw()
        {
            self_destruct( *aliasing::as<joint::ward>(impl) );
            Y_BZSET_STATIC(impl);
        }

        pooled:: pooled() throw()
        {
            Y_BZSET_STATIC(impl);
            new ( aliasing::anonymous(impl) ) joint::ward( Y_CHUNK_SIZE );
        }

        void * pooled:: acquire( size_t &n )
        {
            static joint::ward &w = *aliasing::as<joint::ward>(impl);
            Y_LOCK(access);
            return w.acquire_block(n);
        }

        void pooled:: release(void * &p, size_t &n) throw()
        {
            static joint::ward &w = *aliasing::as<joint::ward>(impl);
            Y_LOCK(access);
            return w.release_block(p,n);
        }

        bool  pooled:: compact(void * &addr, size_t &capa, const size_t size ) throw()
        {
            static joint::ward &w = *aliasing::as<joint::ward>(impl);
            Y_LOCK(access);
            return w.compact(addr,capa,size);
        }

    }
}


#if 0
#include "y/memory/carver.hpp"
#include "y/object.hpp"
#include "y/type/self-destruct.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/block/zset.hpp"

namespace upsylon
{
    namespace memory
    {

        const at_exit::longevity  pooled:: life_time =  object::life_time-1;
        
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
#endif


