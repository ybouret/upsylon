//! \file
#ifndef Y_MEMORY_BLOCKS_INCLUDED
#define Y_MEMORY_BLOCKS_INCLUDED 1

#include "y/os/platform.hpp"
#include "y/memory/arena-impl.hpp"

namespace upsylon
{
    namespace memory
    {
        class arena;
        
        //! allocate blocks of memory, on demand, on the fly
        class blocks
        {
        public:
            //__________________________________________________________________
            //
            // initialization
            //__________________________________________________________________
            blocks(const size_t the_chunk_size); //!< setup
            ~blocks() throw();  //!< cleanup

            //! compute chunk size big enough to run those blocks
            static size_t compute_chunk_size( const size_t the_chunk_size ) throw();

            //__________________________________________________________________
            //
            // run-time
            //__________________________________________________________________
            void * acquire(const size_t block_size);                           //!< aquire n>0 byte
            void   release(void *p, const size_t block_size ) throw();         //!< release a previously allocated bloc_size
            bool   owns(const void *p, const size_t block_size) const throw(); //!< check ownership

            //__________________________________________________________________
            //
            // data
            //__________________________________________________________________
            const size_t   chunk_size;  //!< the common chunk_size
            const size_t   table_mask;  //!< the htable size - 1
        private:
            arena          *acquiring; //!< acquiring cache
            arena          *releasing; //!< releasing cache
            void           *htable;    //!< array of core::list_of<arena>
            void           *z_wksp[Y_MEMORY_ARENA_BLK];  //!< provider of empty arena
            void           *z_impl;
            Y_DISABLE_COPY_AND_ASSIGN(blocks);

        };

    }
}

#endif


