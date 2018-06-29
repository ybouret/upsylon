//! \file
#ifndef Y_MEMORY_BLOCKS_INCLUDED
#define Y_MEMORY_BLOCKS_INCLUDED 1

#include "y/memory/arena.hpp"

namespace upsylon
{
    namespace memory
    {

        class blocks
        {
        public:
            typedef core::list_of<arena> arena_list;
            typedef core::pool_of<arena> arena_pool;

            blocks(const size_t the_max_block_size,
                   const size_t the_chunk_size);
            ~blocks() throw();

            static const size_t bytes_for_htable = sizeof(arena_list);
            static const size_t bytes_for_arenas = 2*sizeof(arena);
            static const size_t min_chunk_size   = (bytes_for_htable>bytes_for_arenas) ? bytes_for_htable : bytes_for_arenas;

            const size_t max_block_size;
            const size_t chunk_size;
            const size_t htable_maxi;
            const size_t htable_size;
            const size_t htable_mask;
            
        private:
            struct page
            {
                page *next;
            };
            arena_list *htable;
            core::pool_of<page> pages; //!< ever growing memory



            Y_DISABLE_COPY_AND_ASSIGN(blocks);
        };

    }
}

#endif


