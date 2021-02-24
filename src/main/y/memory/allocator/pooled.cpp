#include "y/memory/allocator/pooled.hpp"
#include "y/memory/joint/ward.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/collapse.hpp"
#include "y/object-parameters.hpp"
#include <cstring>

namespace upsylon
{
    Y_SINGLETON_IMPL(memory::pooled);
    
    namespace memory
    {


        namespace
        {
            static uint64_t impl[ Y_U64_FOR_ITEM(joint::ward) ] = { 0 };
        }


        pooled:: ~pooled() throw()
        {
            collapse( *aliasing::as<joint::ward>(impl) );
            memset(impl,0,sizeof(impl));
        }

        pooled:: pooled() throw()
        {
            memset(impl,0,sizeof(impl));
            new ( aliasing::anonymous(impl) ) joint::ward( Y_CHUNK_SIZE );
        }

        void * pooled:: acquire( size_t &n )
        {
            if(n>0)
            {
                static joint::ward &w = *aliasing::as<joint::ward>(impl);
                Y_LOCK(access);
                void *p = w.acquire_block(n);
                assert(n>0);
                return p;
            }
            else
            {
                return 0;
            }
        }

        void pooled:: release(void * &p, size_t &n) throw()
        {
            if(p)
            {
                static joint::ward &w = *aliasing::as<joint::ward>(impl);
                Y_LOCK(access);
                w.release_block(p,n);
            }
            else
            {
                assert(0==n);
            }
        }

        bool  pooled:: compact(void * &addr, size_t &capa, const size_t size ) throw()
        {
            static joint::ward &w = *aliasing::as<joint::ward>(impl);
            Y_LOCK(access);
            return w.compact(addr,capa,size);
        }

    }
}


 
