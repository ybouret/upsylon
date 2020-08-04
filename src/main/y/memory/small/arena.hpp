//! \file

#ifndef Y_MEMORY_SMALL_ARENA_INCLUDED
#define Y_MEMORY_SMALL_ARENA_INCLUDED 1


#include "y/memory/small/chunk.hpp"
#include "y/memory/small/zcache.hpp"

#include "y/core/list.hpp"
#include "y/core/pool.hpp"

namespace upsylon {

    namespace memory {

        namespace small {

            //__________________________________________________________________
            //
            //
            //! cache of zombie chunks
            //
            //__________________________________________________________________
            typedef zcache<chunk> zchunks;


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
                arena(const size_t     usr_block_size,
                        const size_t   usr_chunk_size,
                        zchunks       &usr_cache);


                ~arena() throw();

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                void  *acquire();                        //!< allocate a zeroed block
                void   release(void *) throw();          //!< release a previously allocated block
                size_t blocks_per_chunk() const throw(); //!< get acquiring->provided_number

            private:
                chunk               *acquiring; //!< current acquiring piece
                chunk               *releasing; //!< current acquiring piece
                chunk               *empty_one; //!< empty piece
                size_t               available; //!< bookkeeping of available blocks
                core::list_of<chunk> chunks;    //!< pieces, sorted by increasing memory
                zchunks             *shared;    //!< shared cache
                
            public:
                const size_t block_size; //!< the block size
                const size_t chunk_size; //!< clamp( piece::min_chunk_size(block_size), usr_chunk_size, piece::max_chunk_size(block_size) )

            private:
                Y_DISABLE_COPY_AND_ASSIGN(arena);
                chunk *create_chunk();                //!< create an put it in its position, update 'available'
                void   delete_chunk(chunk *p) throw();//!< delete memory and return to cache, update 'available'
            };
        }

    }

}

#endif

