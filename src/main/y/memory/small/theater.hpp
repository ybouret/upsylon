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
                        const size_t usr_chunk_size) throw();

                ~theater() throw();


                //! compute the bytes to hold a valid page and a chunk_size
                /**
                 - compute paging = 2*sizeof(piece)
                 - return next_power_of_two( max_of(paging,usr_chunk_size) )
                 */
                static size_t page_bytes_for(const size_t usr_chunk_size) throw();

                piece *piece_from_new_page();
                piece *query_piece();

                //! acquire a zeroed block
                void  *acquire();

            private:
                piece               *acquiring;
                piece               *releasing;

            public:
                const size_t block_size; //!< the block size
                const size_t chunk_size; //!< clamp( piece::min_chunk_size(block_size), usr_chunk_size, piece::max_chunk_size(block_size) )
                const size_t page_bytes; //!< min_page_bytes(usr_chunk_size)

            private:
                Y_DISABLE_COPY_AND_ASSIGN(theater);

                // internal memory for pieces
                struct page
                {
                    page *next;
                    //page *prev;
                };


                core::list_of<piece> alive; //!< pieces sorted by increasing memory
                core::pool_of<piece> cache; //!< dead pieces
                core::pool_of<page>  pages; //!< ever growing memory for pieces

            public:
                const size_t         ppp;    //!< pieces per page
            };
        }

    }

}

#endif

