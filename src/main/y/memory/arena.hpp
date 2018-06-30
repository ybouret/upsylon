//! \file
#ifndef Y_MEMORY_ARENA_INCLUDED
#define Y_MEMORY_ARENA_INCLUDED 1
#include "y/memory/chunk.hpp"
#include "y/core/list.hpp"
#include "y/core/pool.hpp"

namespace upsylon
{
    namespace memory
    {
        //! default chunk to optimize CHUNK_SIZE up to 128k
        typedef __chunk<uint16_t> chunk;

        //! an arena of chunk
        /**
         delivers blocks of same size (block_size), using allocations
         of only chunk_size data for everything that is needed
         */
        class arena
        {
        public:
            //! initialize arena
            arena(const size_t the_block_size,
                  const size_t the_chunk_size) throw();

            //! release all memory
            ~arena() throw();

            const size_t block_size; //!< the same block_size for everyone
            const size_t chunk_size; //!< global memory chunk_size for any allocation


            //! compute required chunk_size to run this arena
            static size_t compute_chunk_size(const size_t the_block_size,
                                             const size_t the_chunk_size) throw();


            //! acquire one block of block_size
            void *acquire();

            //! release a previously allocated block
            void  release(void *p) throw();

        private:
            //! memory page for chunks mapping
            struct page
            {
                page *next;
            };

            chunk                *acquiring; //!< cache to acquire
            chunk                *releasing; //!< cache to release
            size_t                available; //!< total available blocks
            chunk                *empty;     //!< existent empty
            core::list_of<chunk>  chunks;    //!< live chunks
            core::pool_of<chunk>  cached;    //!< dead chunks
            core::pool_of<page>   pages;     //!< ever growing memory to store initial dead chunks

            Y_DISABLE_COPY_AND_ASSIGN(arena);
            void    load_new_chunk( chunk *node ) throw();
            void    new_block();
            chunk  *new_chunk();
            
        public:
            arena       *next; //!< for list
            arena       *prev; //!< for list
            const size_t chunks_per_page; //!< number of initial dead chunks per memory page
        };

    }
}

#endif

