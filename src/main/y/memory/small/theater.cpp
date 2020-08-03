
#include "y/memory/small/theater.hpp"
#include "y/type/utils.hpp"
#include "y/memory/allocator/global.hpp"
#include "y/os/static-check.hpp"
#include <iostream>

namespace upsylon {

    namespace memory {

        namespace small {

            theater:: ~theater() throw()
            {

                if(pages.size)
                {
                    static global &mgr = global::location();

                    if(alive.size)
                    {
                        for(piece *p=alive.tail;p;p=p->prev)
                        {
                            assert(p->data);
                            mgr.__free(p->data,chunk_size);
                        }
                        alive.reset();
                    }

                    cache.reset();

                    while( pages.size )
                    {
                        mgr.__free( pages.query(), page_bytes );
                    }

                }
                else
                {
                    assert(alive.size==0);
                    assert(cache.size==0);
                }
            }



            static inline
            size_t chunk_size_for(const size_t block_size, const size_t chunk_size)
            {
                assert(block_size>0);
                const size_t min_cs = piece::min_chunk_size_for(block_size);
                const size_t max_cs = piece::max_chunk_size_for(block_size);
                return clamp(min_cs,next_power_of_two(chunk_size),max_cs);
            }

            size_t theater:: page_bytes_for(const size_t usr_chunk_size) throw()
            {
                Y_STATIC_CHECK(sizeof(page)<=sizeof(piece),page_size_too_big);
                static const size_t paging = sizeof(piece)*2;
                return next_power_of_two(max_of(paging,usr_chunk_size));
                
            }
            
            theater:: theater(const size_t usr_block_size,
                              const size_t usr_chunk_size) throw() :
            acquiring(0),
            releasing(0),
            block_size( usr_block_size ),
            chunk_size( chunk_size_for(block_size,usr_chunk_size) ),
            page_bytes( page_bytes_for(chunk_size) ),
            alive(),
            cache(),
            pages(),
            ppp( (page_bytes-sizeof(piece))/sizeof(piece) )
            {

            }

            piece * theater:: piece_from_new_page()
            {
                static global &mgr = global::instance();

                //--------------------------------------------------------------
                // create and store a new page
                //--------------------------------------------------------------
                void  *q = mgr.__calloc(1,page_bytes);
                (void) pages.store(static_cast<page  *>(q));

                //--------------------------------------------------------------
                // use first piece as result
                //--------------------------------------------------------------
                piece *z = static_cast<piece *>(q)+1;

                //--------------------------------------------------------------
                // store following pieces
                //--------------------------------------------------------------
                for(size_t i=1;i<ppp;++i)
                {
                    cache.store(&z[i]);
                }
                return z;
            }

            piece * theater:: query_piece()
            {
                static global &mgr = global::instance();

                piece * p = cache.size ? cache.query() : piece_from_new_page();

                // safely acquire memory for page
                try {
                    (void) alive.push_back( new (p) piece( block_size, mgr.__calloc(1,chunk_size), chunk_size ) );
                }
                catch(...)
                {
                    cache.store(p);
                    throw;
                }

                while(p->prev && p->data<p->prev->data)
                {
                    alive.towards_head(p);
                }
                return p;
            }


            void  * theater:: acquire()
            {
                if(acquiring)
                {

                }
                return NULL;
            }

        }

    }

}


