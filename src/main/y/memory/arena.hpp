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

            //! minimal chunk size, to have at least one chunk per memory block!
            static const size_t min_chunk_size = 2 * sizeof(chunk);

            //! acquire one block of block_size
            void *acquire();

            //! release a previously allocated block
            void  release(void *p) throw();

        private:
            //! memory block for chunks: mapped on a block of chunk_size bytes
            struct mblock
            {
                mblock *next;
            };

            chunk                *acquiring; //!< cache to acquire
            chunk                *releasing; //!< cache to release
            size_t                available; //!< total available blocks
            core::list_of<chunk> chunks;    //!< allocated chunks
            core::pool_of<chunk> cached;    //!< not allocated chunks
            core::pool_of<mblock> mstore;    //!< ever growing memory store

            Y_DISABLE_COPY_AND_ASSIGN(arena);
            void    load_new_chunk( chunk *node ) throw();
            void    new_mblock();
            chunk  *new_chunk();
            
        public:
            const size_t          chunks_per_mblock; //!< number of chunks per memory block

        };

    }
}

#endif

