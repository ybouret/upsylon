#include "y/memory/blocks.hpp"
#include "y/memory/global.hpp"
#include "y/type/utils.hpp"

#include <iostream>

namespace upsylon
{
    namespace memory
    {
        void blocks:: __release_pages_and_table() throw()
        {
            while(pages.size)
            {
                global::location().__free( pages.query(), chunk_size );
            }
            global::location().__free(htable,chunk_size);
        }

        blocks:: ~blocks() throw()
        {
            for(size_t i=0;i<=table_mask;++i)
            {
                arena_list &slot = htable[i];
                while(slot.size)
                {
                    arena *a = slot.pop_back();
                    destruct(a);
                }
                slot.reset();
            }
            cached.reset();
            __release_pages_and_table();
        }

        size_t blocks:: compute_chunk_size(const size_t the_chunk_size) throw()
        {
            //! memory to hold a prime number of arena_list
            static const size_t min_htable_size = 2 * sizeof(arena_list);
            size_t cs = max_of(the_chunk_size,min_htable_size);

            //! memory to hold at least a page address and an arena
            static const size_t min_page_size = sizeof(void*) + sizeof(arena);
            cs = max_of(cs,min_page_size);

            //! and get the chunk size
            return next_power_of_two(cs);
        }

#define Y_BLOCKS_NEW_ARENA()              \
static global &hmem = global::location(); \
arena *a = io::cast<arena>(pages.store( static_cast<page *>( hmem.__calloc(1,chunk_size) )),sizeof(void*));\
for(size_t i=0;i<arenas_per_page;++i) cached.store(a+i)

        blocks:: blocks(const size_t the_chunk_size,
                        const size_t initial_arenas) :
        chunk_size( compute_chunk_size(the_chunk_size) ),
        table_mask( most_significant_bit_mask(chunk_size/sizeof(arena_list))-1 ),
        acquiring(0),
        releasing(0),
        htable( static_cast<arena_list *>(global::instance().__calloc(1, chunk_size)) ),
        cached(),
        pages(),
        arenas_per_page( (chunk_size-sizeof(void*))/sizeof(arena) )
        {
#if 0
            std::cerr << "chunk_size      =" << chunk_size  << "/" << the_chunk_size << std::endl;
            std::cerr << "table_maxi      =" << chunk_size/sizeof(arena_list) << std::endl;
            std::cerr << "table_size      =" << table_mask+1 << std::endl;
            std::cerr << "arenas_per_page =" << arenas_per_page << std::endl;
#endif
            try
            {
                while(cached.size<initial_arenas)
                {
                    Y_BLOCKS_NEW_ARENA();
                }
            }
            catch(...)
            {
                __release_pages_and_table();
                throw;
            }
        }

        
    }
}

namespace upsylon
{
    namespace memory
    {

        void *blocks:: acquire(const size_t block_size)
        {
            assert(block_size>0);
            arena_list &slot = htable[ block_size & table_mask ];
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
                // need to take from cached
                //______________________________________________________________
                if(cached.size<=0)
                {
                    Y_BLOCKS_NEW_ARENA();
                }

                assert(cached.size>0);
                arena *a = cached.query();
                try { new (a) arena(block_size,chunk_size); }
                catch(...){ cached.store(a); throw;}
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
            arena_list &slot = htable[ block_size & table_mask ];
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
            arena_list &slot = htable[ block_size & table_mask ];
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

