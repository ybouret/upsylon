
//! \file

#ifndef Y_MEMORY_TIGHT_BLOCKS_INCLUDED
#define Y_MEMORY_TIGHT_BLOCKS_INCLUDED 1

#include "y/memory/tight/arena.hpp"


namespace upsylon {

    namespace memory {

        namespace tight {

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

                //! create internal structure
                /**
                 \param usr_chunk_size the minimal chunk_size for internal table, caches and arena
                 \param usr_limit_size the size above which memory is asked directly to system
                 \param usr_sys_quarry a quarry to setup all the caches
                 */
                blocks(const size_t usr_chunk_size,
                       const size_t usr_limit_size,
                       quarry      &usr_sys_quarry);

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

                //! display
                friend std::ostream & operator<<( std::ostream &os, const blocks &b);

                //! get/create arena using query(block_size)
                arena & operator[](const size_t block_size);

                //! get an existing arena
                const arena & operator[](const size_t block_size) const;

                //! try to compact a previously allocated block
                bool try_compact(void * &addr, const size_t block_size) throw();

                //! find ownership and block size
                bool owns(const void *addr, size_t &block_size) const throw();

                //______________________________________________________________
                //
                // members
                //______________________________________________________________

                size_t       oversized;  //!< local count of above limit
                const size_t chunk_size; //!< next power of two(max_of(usr_chunk_size,min_chunk_size,limit_size))
                const size_t limit_size; //!< will call global allocator above this size
                const size_t slots_size; //!< number of slots as a power of two
                const size_t slots_mask; //!< slots_size-1

            private:
                Y_DISABLE_COPY_AND_ASSIGN(blocks);
                vein           &slots_vein;
                slot_type      *slot;
                arena          *acquiring;
                arena          *releasing;
                arena          *query(const size_t block_size);

            public:
                quarry         &sharedQ;      //!< shared quarry for all the internal chunk_size(s)
                zcache<chunk>   zChunks;      //!< all the zombie chunks for all the live arenas
                zcache<arena>   zArenas;      //!< all the zombie arenas for the internal hash table
             };


        }

    }

}

#endif

