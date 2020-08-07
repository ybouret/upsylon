
#include "y/memory/small/objects.hpp"
#include "y/memory/allocator/global.hpp"
#include "y/type/block/zset.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon {

    namespace memory {

        namespace small {

            objects:: ~objects() throw()
            {
                Y_BZSET_STATIC(little);
            }

            objects:: objects(lockable &sync,const size_t chunk_size, const size_t limit_size) :
            Access(sync),
            Quarry(),
            Blocks(chunk_size,limit_size,Quarry),
            little()
            {
                Y_BZSET_STATIC(little);
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

            void *objects:: dyadic_acquire(const size_t block_exp2)
            {
                if(block_exp2>vein::max_exp2)
                {
                    throw libc::exception(EDOM,"small::objects::dyadic_acquire(block_exp2=%u>%u)",unsigned(block_exp2),unsigned(vein::max_exp2));
                }

                if(block_exp2<vein::min_exp2)
                {
                    arena **     ppA = &little[block_exp2];
                    if(!(*ppA)) *ppA = &Blocks[1<<block_exp2];
                    return    (**ppA).acquire();
                }
                else
                {
                    return Quarry[block_exp2].acquire();
                }
            }

            void objects:: dyadic_release(void *addr, const size_t block_exp2) throw()
            {
                assert(addr);
                assert(block_exp2<=vein::max_exp2);

                if(block_exp2<vein::min_exp2)
                {
                    assert(NULL!=little[block_exp2]);
                    assert( (1<<block_exp2) == little[block_exp2]->block_size );
                    little[block_exp2]->release(addr);
                }
                else
                {
                    Quarry[block_exp2].release(addr);
                }
            }

        }

    }

}

