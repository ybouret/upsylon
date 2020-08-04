
//! \file

#ifndef Y_MEMORY_SMALL_BLOCKS_INCLUDED
#define Y_MEMORY_SMALL_BLOCKS_INCLUDED 1

#include "y/memory/small/arena.hpp"


namespace upsylon {

    namespace memory {

        namespace small {

            //__________________________________________________________________
            //
            //
            //! hash table of arena for block_size allocation dispatching
            //
            //__________________________________________________________________
            class blocks
            {
            public:
                //______________________________________________________________
                //
                // types and definition
                //______________________________________________________________
                typedef core::list_of<arena> slot_type;                          //!< alias, for chaining
                static const size_t          min_chunk_size = sizeof(slot_type); //!< need at least a slot

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! build
                blocks(const size_t usr_chunk_size,
                       const size_t usr_limit_size);

                //! clean all resources
                ~blocks() throw();

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! acquire a new zeroed block_size
                void *acquire(const size_t block_size);

                //! release a previously allocated block_size
                void  release(void *addr, const size_t block_size) throw();

                //! compute load_factor=limit_size/slot_size
                size_t load_factor() const throw();

                //______________________________________________________________
                //
                // members
                //______________________________________________________________

                const size_t chunk_size; //!< next power of two(max_of(usr_chunk_size,min_chunk_size,limit_size))
                const size_t slots_size; //!< number of slots as a power of two
                const size_t slots_mask; //!< slots_size-1
                const size_t limit_size; //!< will call global allocator above this size


            private:
                Y_DISABLE_COPY_AND_ASSIGN(blocks);
                slot_type      *slot;
                arena          *acquiring;
                arena          *releasing;

            public:
                zcache<chunk>   chunks;      //!< shared zombie chunks, built with chunk_size
                zcache<arena>   arenas;      //!< shared zombie arenas, built with chunk_size
             };


        }

    }

}

#endif

