//! \file
#ifndef Y_MEMORY_BLOCKS_INCLUDED
#define Y_MEMORY_BLOCKS_INCLUDED 1

#include "y/memory/arena.hpp"

namespace upsylon
{
    namespace memory
    {

        //! allocate blocks of memory, on demand, on the fly
        class blocks
        {
        public:
            typedef core::list_of<arena> arena_list; //!< to store live arenas
            typedef core::pool_of<arena> arena_pool; //!< to store dead arenas

            //! intialize the htable
            blocks(const size_t the_chunk_size,
                   const size_t initial_arenas=0);

            //! cleanup
            ~blocks() throw();

            //! compute chunk size big enough to run those blocks
            static size_t compute_chunk_size( const size_t the_chunk_size ) throw();

            const size_t chunk_size;  //!< the common chunk_size
            const size_t table_mask;  //!< the htable size - 1
            
            void * acquire(const size_t block_size);                   //!< aquire n>0 byte
            void   release(void *p, const size_t block_size ) throw(); //!< release a previously allocated bloc_size
            bool   owns(const void *p, const size_t block_size) const throw(); //!< check ownership

        private:
            struct page
            {
                page *next;
            };
            arena              *acquiring; //!< acquiring cache
            arena              *releasing; //!< releasing cache
            arena_list         *htable;    //!< arena_list
            arena_pool          cached;    //!< empty arenas, mapped from pages
            core::pool_of<page> pages;     //!< ever growing memory of chunk_size to hold dead arenas
            Y_DISABLE_COPY_AND_ASSIGN(blocks);

            void __release_pages_and_table() throw();

        public:
            const size_t arenas_per_page; //!< how many dead arenas are cached from one page of length chunk_size

            //! how many pages are in memory
            inline size_t num_pages()  const throw() { return pages.size; }

            //! how many arenas (a.k.a different block_size) are held
            inline size_t num_arenas() const throw() { size_t ans = 0; for(size_t i=0;i<=table_mask;++i) { ans += htable[i].size; } return ans; }
            
        };

    }
}

#endif


