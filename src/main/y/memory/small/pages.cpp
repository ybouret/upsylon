
#include "y/memory/small/pages.hpp"
#include "y/type/utils.hpp"
#include "y/memory/allocator/global.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {

    namespace memory {

        namespace small {

            pages:: ~pages() throw()
            {
            }
            
            size_t pages:: chunk_size_for(const size_t usr_chunk_size) throw()
            {
                const size_t bytes_for_pieces = min_pieces_per_page * sizeof(piece) + header_size;
                return next_power_of_two( max_of(usr_chunk_size,bytes_for_pieces) );
            }

            pages:: pages(const size_t usr_chunk_size) throw() :
            chunk_size( chunk_size_for(usr_chunk_size) ),
            pieces_per_page( (chunk_size-header_size)/sizeof(piece) ),
            shared(),
            zstore()
            {
                
            }

            piece * pages:: query_from_new_page()
            {
                static global &mgr = mgr.instance();

                assert(shared.size<=0);

                // get memory
                void *buffer = mgr.__calloc(1,chunk_size);

                // store it as a page
                (void) zstore.store( static_cast<page *>(buffer) );

                // use extra memory as pieces
                piece *p = aliasing::forward<piece>(buffer,header_size);
                for(size_t i=1;i<pieces_per_page;++i)
                {
                    shared.push_back( &p[i] );
                }
                return p;
            }

            void pages:: store_nil(piece *p) throw()
            {
                assert(p);
                assert(0==p->next);
                assert(0==p->prev);
                shared.push_front(p);
            }

            piece *pages:: query_nil()
            {
                return shared.size ? shared.pop_front() : query_from_new_page();
            }
            

        }

    }

}

