#ifndef Y_MEMORY_ARENA_INCLUDED
#define Y_MEMORY_ARENA_INCLUDED 1

#include "y/memory/chunk.hpp"
#include "y/core/list.hpp"
#include "y/core/pool.hpp"

namespace upsylon
{
    namespace memory
    {
        typedef chunk<uint16_t> mchunk;

        class arena
        {
        public:
            explicit arena(const size_t the_block_size,
                           const size_t the_chunk_size) throw();
            virtual ~arena() throw();

            const size_t block_size;
            const size_t chunk_size;

            static const size_t min_chunk_size = 2 * sizeof(mchunk);

            //! acquire one block of block_size
            void *acquire();

        private:
            //! memory block for chunks: mapped on a block of chunk_size bytes
            struct mblock
            {
                mblock *next;
            };

            mchunk               *acquiring; //!< cache to acquire
            size_t                available; //!< total available blocks
            core::list_of<mchunk> chunks;    //!< allocated chunks
            core::pool_of<mchunk> cached;    //!< not allocated chunks
            core::pool_of<mblock> mstore;    //!< ever growing memory store

            Y_DISABLE_COPY_AND_ASSIGN(arena);
            void load_new_chunk( mchunk *node ) throw();
            void    new_mblock();
            mchunk *new_mchunk();
            
        public:
            const size_t          chunks_per_mblock;

        };

    }
}

#endif

