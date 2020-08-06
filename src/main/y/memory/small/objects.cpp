
#include "y/memory/small/objects.hpp"
#include "y/memory/allocator/global.hpp"

namespace upsylon {

    namespace memory {

        namespace small {

            objects:: ~objects() throw()
            {

            }

            objects:: objects(lockable &sync,const size_t chunk_size, const size_t limit_size) :
            Access(sync),
            Quarry(),
            Blocks(chunk_size,limit_size,Quarry)
            {

            }

            void * objects:: acquire(const size_t block_size)
            {
                Y_LOCK(Access);
                return Blocks.acquire(block_size);
            }

            void objects:: release(void *addr, const size_t block_size) throw()
            {
                Y_LOCK(Access);
                Blocks.release(addr,block_size);
            }

            void *objects:: global_acquire(const size_t global_bytes)
            {
                static global &G = global::instance();
                return G.__calloc(1,global_bytes);
            }

            void objects:: global_release(void *addr, const size_t global_bytes) throw()
            {
                static global &G = global::location();
                G.__free(addr,global_bytes);
            }

        }

    }

}

