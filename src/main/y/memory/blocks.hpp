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

            blocks(const size_t the_chunk_size);
            ~blocks() throw();

            static const size_t bytes_for_htable = sizeof(arena_list); //! at least one arena_list for the htable
            static const size_t bytes_for_arenas = 2*sizeof(arena);    //! at least one arena in a blocks::page
            static const size_t min_chunk_size   = (bytes_for_htable>bytes_for_arenas) ? bytes_for_htable : bytes_for_arenas;

            const size_t chunk_size;
            const size_t htable_size;
            const size_t htable_mask;
            
            void * acquire(const size_t n);
            void   release(void *p, const size_t block_size ) throw();
            
        private:
            struct page
            {
                page *next;
            };
            arena              *acquiring;
            arena              *releasing;
            arena_list         *htable;
            arena_pool          cached; //!< empty arenas
            core::pool_of<page> pages;  //!< ever growing memory of chunk_size to hold empty arenas
            Y_DISABLE_COPY_AND_ASSIGN(blocks);

        public:
            const size_t arenas_per_page;
        };

    }
}

#endif


