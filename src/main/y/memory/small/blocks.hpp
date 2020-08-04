
//! \file

#ifndef Y_MEMORY_SMALL_BLOCKS_INCLUDED
#define Y_MEMORY_SMALL_BLOCKS_INCLUDED 1


#include "y/memory/small/arena.hpp"


namespace upsylon {

    namespace memory {

        namespace small {


            class blocks
            {
            public:
                typedef core::list_of<arena> slot_type;
                static const size_t          min_chunk_size = sizeof(slot_type);

                blocks(const size_t usr_chunk_size,
                       const size_t usr_limit_size);
                ~blocks() throw();

                const size_t chunk_size; //!< next power of two usr_chunk_size
                const size_t slots_size; //!< number of slots, power of two
                const size_t slots_mask; //!< slots_size-1
                const size_t limit_size; //!< will call global allocator above this size



            private:
                Y_DISABLE_COPY_AND_ASSIGN(blocks);
            };


        }

    }

}

#endif

