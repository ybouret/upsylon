//! \file

#ifndef Y_MEMORY_TIGHT_ARENA_INCLUDED
#define Y_MEMORY_TIGHT_ARENA_INCLUDED 1


#include "y/memory/tight/chunk.hpp"
#include "y/core/list.hpp"
#include <iosfwd>

namespace upsylon {

    namespace memory {

        namespace tight {

            class quarry; //!< forward declaration
            class vein;   //!< forward declaration
            
            //__________________________________________________________________
            //
            //
            //! arena of multiple chunks to acquire/release same block_size
            //
            //__________________________________________________________________
            class arena
            {
            public:

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                //! setup with first acquiring, releasing and empty_one
                /**
                 \param the_block_size the block size for all chunks
                 \param req_chunk_size the requested chunk size
                 \param shared_zchunks address of a cache for zombie chunks
                 \param shared_quarry  a cache for power of two chunk_data

                 - min_cs = max_of(chunk::min_chunk_size_for(block_size),vein::min_bytes)
                 - max_cs = max_of(chunk::max_chunk_size_for(block_size),min_cs);
                 - compute chunk_exp2 and keep a reference of the associated stones
                 */
                arena(const size_t   the_block_size,
                      const size_t   req_chunk_size,
                      void *         shared_zchunks,
                      quarry        &shared_quarry);

                //! cleanup
                ~arena() throw();

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                void  *acquire();                        //!< allocate a zeroed block
                void   release(void *) throw();          //!< release a previously allocated block
                
                //! try to move content in a lower chunk
                bool compact(void * &addr) throw();

                //! display
                friend std::ostream & operator<<(std::ostream &, const arena &);

                //! check ownership
                bool owns(const void *addr) const throw();

                //! trigger zchunks garbage collector
                void gc() throw();

            private:
                chunk               *acquiring; //!< current acquiring piece
                chunk               *releasing; //!< current acquiring piece
                chunk               *empty_one; //!< empty chunk
                size_t               available; //!< bookkeeping of available blocks
                core::list_of<chunk> chunks;    //!< chunks, sorted by increasing memory

            public:
                const size_t block_size; //!< the block size
                const size_t chunk_size; //!< adapted for block_size
                arena       *next;       //!< for list
                arena       *prev;       //!< for list
                
            private:
                void          *zchunks;   //!< shared cache of zombie chunks
                vein          &deposit;   //!< shared cache of ingots

            public:
                const size_t   blocks_per_chunk; //!< acquiring->provided_number
                const size_t   reserved;         //!< sizeof(arena)=64/128
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(arena);
                chunk *create_chunk();                       //!< create an put it in its position, update 'available'
                void   delete_chunk(chunk *)      throw();   //!< delete memory and return to cache, update 'available'
                void   releasing_at(const void *) throw();   //!< move releasing to designated address
            };
        }

    }

}

#endif

