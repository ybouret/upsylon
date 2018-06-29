#include "y/memory/blocks.hpp"
#include "y/type/utils.hpp"
#include "y/memory/global.hpp"
#include <iostream>

namespace upsylon
{
    namespace memory
    {
        blocks:: ~blocks() throw()
        {
            for(size_t i=0;i<htable_size;++i)
            {
                arena_list &slot = htable[i];
                while(slot.size)
                {
                    arena *a = slot.pop_back();
                    std::cerr << "remove arena.block_size=" << a->block_size << std::endl;
                    destruct(a);
                }
                slot.reset();
            }
            cached.reset();
            while(pages.size)
            {
                global::location().__free( pages.query(), chunk_size );
            }
            global::location().__free(htable,chunk_size);
        }

        blocks:: blocks(const size_t the_chunk_size) :
        chunk_size( max_of(the_chunk_size,min_chunk_size) ),
        htable_size( most_significant_bit_mask(chunk_size/sizeof(arena_list)) ),
        htable_mask(htable_size-1),
        acquiring(0),
        releasing(0),
        htable( static_cast<arena_list *>(global::instance().__calloc(1, chunk_size)) ),
        pages(),
        arenas_per_page(chunk_size/sizeof(arena)-1)
        {
            std::cerr << "chunk_size      =" << chunk_size  << std::endl;
            std::cerr << "htable_maxi     =" << chunk_size/sizeof(arena_list) << std::endl;
            std::cerr << "htable_size     =" << htable_size << std::endl;
            std::cerr << "htable_mask     =" << htable_mask << std::endl;
            std::cerr << "arenas_per_page =" << arenas_per_page << std::endl;
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
            if(acquiring && (acquiring->block_size==block_size) )
            {
                // cached!
                return acquiring->acquire();
            }
            else
            {
                // locate where arena should be
                arena_list *slot = & htable[ block_size & htable_mask ];

                // look up
                for(arena *a=slot->head;a;a=a->next)
                {
                    if(block_size==a->block_size)
                    {
                        // found
                        acquiring = a;
                        return a->acquire();
                    }
                }

                // need a new arena!
                if(cached.size<=0)
                {
                    // need new empty arenas
                    page *p = static_cast<page *>( global::instance().__calloc(1,chunk_size) );
                    pages.store(p);
                    arena *a = io::cast<arena>(p,sizeof(arena));
                    for(size_t i=0;i<arenas_per_page;++i)
                    {
                        cached.store(a+i);
                    }
                }
                assert(cached.size>0);
                arena *a = cached.query();
                try { new (a) arena(block_size,chunk_size); }
                catch(...){ cached.store(a); throw;}
                slot->push_front(a);
                acquiring = a;
                return a->acquire();
            }
        }
    }

}

