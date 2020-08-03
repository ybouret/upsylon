
//! \file

#ifndef Y_MEMORY_SMALL_PAGES_INCLUDED
#define Y_MEMORY_SMALL_PAGES_INCLUDED 1

#include "y/memory/small/piece.hpp"
#include "y/core/pool.hpp"
#include "y/core/list.hpp"

namespace upsylon {

    namespace memory {

        namespace small {


            //__________________________________________________________________
            //
            //
            //! provider of blank pieces
            //
            //__________________________________________________________________
            class pages
            {
            public:
                static const size_t header_size         = sizeof(void*);
                static const size_t min_pieces_per_page = 2;

                //! enough memory for some pieces and a page
                /**
                 [page *next;...;piece[0]..piece[pieces_per_page-1]
                 */
                static size_t chunk_size_for(const size_t usr_chunk_size) throw();

                pages(const size_t usr_chunk_size) throw();
                ~pages() throw();

                const size_t chunk_size;
                const size_t pieces_per_page;

                void   store_nil(piece *p) throw();
                piece *query_nil();
                void   gc() throw();


            private:
                Y_DISABLE_COPY_AND_ASSIGN(pages);
                struct page
                {
                    page *next;
                };
                piece *query_from_new_page();
                core::list_of<piece> pieces; //!< L.R.U cache
                core::pool_of<page>  zstore; //!< ever growing store
                bool is_busy(const page *) const throw();

            };

        }

    }

}

#endif
