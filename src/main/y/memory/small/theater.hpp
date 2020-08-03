//! \file

#ifndef Y_MEMORY_SMALL_THEATER_INCLUDED
#define Y_MEMORY_SMALL_THEATER_INCLUDED 1


#include "y/memory/small/pages.hpp"
#include "y/core/list.hpp"
#include "y/core/pool.hpp"

namespace upsylon {

    namespace memory {

        namespace small {

            class theater
            {
            public:
                theater(const size_t usr_block_size,
                        const size_t usr_chunk_size,
                        pages       &cache);

                ~theater() throw();


                void  *acquire();                //!< allocate a zeroed block
                void   release(void *p) throw(); //!< release a previously allocated block


            private:
                piece               *acquiring; //!< current acquiring piece
                piece               *releasing; //!< current acquiring piece
                piece               *empty_one; //!< empty piece
            public:
                const size_t         available; //!< bookkeeping of available blocks
            private:
                core::list_of<piece> pieces;    //!< pieces, sorted by increasing memory
                pages               *shared;    //!< shared cache
                
            public:
                const size_t block_size; //!< the block size
                const size_t chunk_size; //!< clamp( piece::min_chunk_size(block_size), usr_chunk_size, piece::max_chunk_size(block_size) )

            private:
                Y_DISABLE_COPY_AND_ASSIGN(theater);
                piece *create_piece();                //!< create an put it in its position, update 'available'
                void   delete_piece(piece *p) throw();//!< delete memory and return to cache, update 'available'
            };
        }

    }

}

#endif

