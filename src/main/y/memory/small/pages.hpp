
//! \file

#ifndef Y_MEMORY_SMALL_PAGES_INCLUDED
#define Y_MEMORY_SMALL_PAGES_INCLUDED 1

#include "y/memory/small/chunk.hpp"
#include "y/core/pool.hpp"
#include "y/core/list.hpp"

namespace upsylon {

    namespace memory {

        namespace small {


            //__________________________________________________________________
            //
            //
            //! provider of blank pieces, a.k.a shared cache for arenas
            //
            //__________________________________________________________________
            class pages
            {
            public:
                static const size_t header_size         = sizeof(void*);
                static const size_t min_pieces_per_page = 2;

                //! enough memory for some chunks and a page
                /**
                 [page *next;...;chunk[0]..chunk[chunks_per_page-1]
                 */
                static size_t chunk_size_for(const size_t usr_chunk_size) throw();

                pages(const size_t usr_chunk_size) throw();
                ~pages() throw();

                const size_t chunk_size;
                const size_t chunks_per_page;

                void   store_nil(chunk *p) throw();
                chunk *query_nil();
                void   gc() throw();


            private:
                Y_DISABLE_COPY_AND_ASSIGN(pages);
                struct page
                {
                    page *next;
                };
                chunk *query_from_new_page();
                core::list_of<chunk> chunks; //!< L.R.U cache
                core::pool_of<page>  zstore; //!< ever growing store
                bool is_busy(const page *) const throw();

            };

        }

    }

}

#endif
