#include "y/memory/blocks.hpp"
#include "y/memory/allocator/global.hpp"
#include "y/type/utils.hpp"
#include "y/code/base2.hpp"
#include "y/os/static-check.hpp"
#include "y/memory/arena-of.hpp"
#include "y/type/self-destruct.hpp"

#include <cstring>

namespace upsylon
{
    namespace memory
    {
        namespace
        {
            typedef core::list_of<arena> arena_list; //!< to store live arenas
            typedef arena_of<arena>      cache_type; //!< to provide dead arenas...
        }

        blocks:: ~blocks() throw()
        {
            static global &mgr = global::location();
            {
                arena_list *slots = static_cast<arena_list *>(htable);
                cache_type *cache = (cache_type *)cached;
                for(size_t i=0;i<=table_mask;++i)
                {
                    arena_list &slot = slots[i];
                    while(slot.size)
                    {
                        arena *a = slot.pop_back();
                        self_destruct(*a);
                        cache->release(a);
                    }
                }
                self_destruct(*cache);
                memset(hidden,0,sizeof(hidden));
            }
            mgr.__free(htable,chunk_size);
        }

        size_t blocks:: compute_chunk_size(const size_t the_chunk_size) throw()
        {
            //------------------------------------------------------------------
            //! memory to hold at least one arena_list
            //------------------------------------------------------------------
            static const size_t min_htable_bytes =  2*sizeof(arena_list);
            size_t cs = max_of(the_chunk_size,min_htable_bytes);

            //------------------------------------------------------------------
            // and use a power of two, since will use htable
            //------------------------------------------------------------------
            return next_power_of_two(cs);
        }


        blocks:: blocks(const size_t the_chunk_size) :
        chunk_size( compute_chunk_size(the_chunk_size) ),
        table_mask( most_significant_bit(chunk_size/sizeof(arena_list))-1 ),
        acquiring(0),
        releasing(0),
        htable( static_cast<arena_list *>(global::instance().__calloc(1, chunk_size)) ),
        cached(0),
        hidden()
        {
            Y_STATIC_CHECK(sizeof(hidden)>=sizeof(cache_type),workspace_too_small);
            assert( is_a_power_of_two(chunk_size)   );
            assert( is_a_power_of_two(table_mask+1) );
            memset(hidden,0,sizeof(hidden));
            cached = (void *)&hidden[0];
            new (cached) cache_type(chunk_size);
        }

        
    }
}

namespace upsylon
{
    namespace memory
    {
#define Y_MEMORY_BLOCK_SLOT() *(static_cast<arena_list *>(htable)+(block_size&table_mask))

        void *blocks:: acquire(const size_t block_size)
        {
            assert(block_size>0);
            arena_list &slot = Y_MEMORY_BLOCK_SLOT();
            if( !acquiring || (block_size!=acquiring->block_size) )
            {
                //______________________________________________________________
                //
                // need to look up
                //______________________________________________________________
                for(arena *a=slot.head;a;a=a->next)
                {
                    if(block_size==a->block_size)
                    {
                        acquiring = a;
                        slot.move_to_front(a);
                        goto ACQUIRE;
                    }
                }

                //______________________________________________________________
                //
                // need to take from zcache
                //______________________________________________________________
                cache_type &zcache = *(cache_type *)(cached);
                arena      *a      = zcache.acquire();
                try { new (a) arena(block_size,chunk_size); }
                catch(...){ zcache.release(a); throw;}
                slot.push_front(a);
                acquiring = a;
            }

        ACQUIRE:
            assert(acquiring);
            assert(block_size==acquiring->block_size);
            return acquiring->acquire();
        }
    }

}

#include "y/os/error.hpp"

namespace upsylon
{
    namespace memory
    {
        void blocks:: release(void *p, const size_t block_size ) throw()
        {
            assert(p);
            assert(block_size>0);
            arena_list &slot = Y_MEMORY_BLOCK_SLOT();
            if(!releasing||(block_size!=releasing->block_size))
            {
                // look up
                for(arena *a=slot.head;a;a=a->next)
                {
                    if(block_size==a->block_size)
                    {
                        releasing = a;
                        goto RELEASE;
                    }
                }
                fatal_error("invalid address/block_size in blocks.release");
            }
        RELEASE:
            assert(releasing);
            assert(block_size==releasing->block_size);
            slot.move_to_front(releasing);
            releasing->release(p);
        }

        bool   blocks:: owns(const void *p, const size_t block_size) const throw()
        {
            assert(p);
            assert(block_size);
            arena_list &slot = Y_MEMORY_BLOCK_SLOT();
            for(arena *a=slot.head;a;a=a->next)
            {
                if(block_size==a->block_size)
                {
                    // found possible releasing arena
                    return a->owns(p);
                }
            }
            return false;
        }


    }

}

