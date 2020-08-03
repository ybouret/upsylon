
#include "y/memory/small/pages.hpp"
#include "y/type/utils.hpp"
#include "y/memory/allocator/global.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/block/zset.hpp"
#include "y/sort/merge.hpp"

#include <iostream>

namespace upsylon {

    namespace memory {

        namespace small {

            pages:: ~pages() throw()
            {
                if(pieces.size!=pieces_per_page*zstore.size)
                {
                    std::cerr << "[small::pages] not all small::pieces are released" << std::endl;
                }
                pieces.reset();
                while(zstore.size)
                {
                    static global &mgr = global::instance();
                    mgr.__free(  zstore.query(), chunk_size );
                }
            }
            
            size_t pages:: chunk_size_for(const size_t usr_chunk_size) throw()
            {
                const size_t bytes_for_pieces = min_pieces_per_page * sizeof(piece) + header_size;
                return next_power_of_two( max_of(usr_chunk_size,bytes_for_pieces) );
            }

            pages:: pages(const size_t usr_chunk_size) throw() :
            chunk_size( chunk_size_for(usr_chunk_size) ),
            pieces_per_page( (chunk_size-header_size)/sizeof(piece) ),
            pieces(),
            zstore()
            {
                
            }

            piece * pages:: query_from_new_page()
            {
                static global &mgr = mgr.instance();

                assert(pieces.size<=0);

                // get memory
                void *buffer = mgr.__calloc(1,chunk_size);

                // store it as a page
                (void)zstore.store( static_cast<page *>(buffer) );

                // use extra memory as pieces
                piece *p = aliasing::forward<piece>(buffer,header_size);
                for(size_t i=1;i<pieces_per_page;++i)
                {
                    assert(is_zeroed(p[i]));
                    pieces.push_back(&p[i]);
                }
                return p;
            }

            void pages:: store_nil(piece *p) throw()
            {
                assert(p);
                assert(0==p->next);
                assert(0==p->prev);
                aliasing::_(p->provided_number) = 0;
                pieces.push_front(p);
            }

            piece *pages:: query_nil()
            {
                if(pieces.size)
                {
                    piece *p =  pieces.pop_front();
                    bzset(*p);
                    return p;
                }
                else
                {
                    piece *p = query_from_new_page();
                    assert(is_zeroed(*p));
                    return p;
                }
            }

            bool pages:: is_busy(const page *p) const throw()
            {
                const piece *z = aliasing::forward<piece>(p,header_size);
                for(size_t i=pieces_per_page;i>0;--i,++z)
                {
                    if(z->provided_number) return true;
                }
                return false;
            }

            void pages:: gc() throw()
            {
                merging<piece>::sort_by_addr(pieces);
            }


        }

    }

}

