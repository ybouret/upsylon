#include "y/memory/arena.hpp"
#include "y/type/utils.hpp"
#include "y/memory/allocator/global.hpp"
#include "y/os/error.hpp"

#include <iostream>

namespace upsylon {

    namespace memory
    {


        chunk * arena:: delete_chunk_data(chunk *node) throw()
        {
            assert(node);
            static global &_ = global::location();
            if( !node->is_empty() )
            {
                const size_t nalloc = node->allocated();
                const size_t bs     = node->words_increment * node->word_size;
                std::cerr << "[memory.chunk] still #allocated=" << nalloc << ", block_size<=" << bs << std::endl;
            }
            _.__free(node->data,aliasing::delta(node->data,node->last));
            return node;
        }

        ////////////////////////////////////////////////////////////////////////
        //
        // arena destruction
        //
        ////////////////////////////////////////////////////////////////////////
        arena:: ~arena() throw()
        {
            if(pages.size)
            {
                assert(global::exists());
                static global &_ = global::location();

                //______________________________________________________________
                //
                // hard reset cached, for these are only memory locations
                //______________________________________________________________
                cached.reset();

                //______________________________________________________________
                //
                // clean allocated chunks
                //______________________________________________________________
                for(chunk *node=chunks.head;node;node=node->next)
                {
                    (void) delete_chunk_data(node);
                }
                // then hard reset
                chunks.reset();

                //______________________________________________________________
                //
                // remove effective storage
                //______________________________________________________________
                while(pages.size)
                {
                    _.__free(pages.query(),chunk_size);
                }

            }
            else
            {
                assert(0==cached.size);
                assert(0==chunks.size);
            }
        }

        size_t arena::compute_chunk_size(const size_t the_block_size,
                                         const size_t the_chunk_size) throw()
        {
            assert(the_block_size>0);

            // chunk size must be bigger than the_block_size and the chunk::word_size
            size_t cs = max_of(the_block_size,the_chunk_size,chunk::word_size);

            // we also need to have at least one sizeof(chunk) in memory page
            const size_t min_page_size = sizeof(void*)+sizeof(chunk);
            cs = max_of(cs,min_page_size);

            // and we align memory
            return next_power_of_two(cs);
        }



        arena:: arena(const size_t the_block_size,
                      const size_t the_chunk_size) throw() :
        block_size(the_block_size),
        chunk_size( compute_chunk_size(block_size,the_chunk_size) ),
        acquiring(0),
        releasing(0),
        available(0),
        empty(0),
        chunks(),
        cached(),
        pages(),
        next(0),
        prev(0),
        chunks_per_page( (chunk_size-sizeof(void*))/sizeof(chunk) )
        {
            assert(block_size>0);
            assert(chunks_per_page>0);
        }

        void arena:: prepare_page()
        {
            static global &hmem = global::instance();
            chunk         *ch   = aliasing::shifted<chunk>(pages.store( static_cast<page *>(hmem.__calloc(1,chunk_size)) ), sizeof(void*) );
            
            for(size_t i=0;i<chunks_per_page;++i)
            {
                cached.store( ch++ );
            }
        }

        void arena:: insert_chunk(chunk *node) throw()
        {
            Y_CORE_CHECK_LIST_NODE(node);

            if(chunks.size<=0)
            {
                chunks.push_back(node);
                return;
            }

            assert(chunks.size>0);

            if(node->data>chunks.tail->data)
            {
                chunks.push_back(node);
                return;
            }

            if(node->data<chunks.head->data)
            {
                chunks.push_front(node);
                return;
            }
            assert(node->data>chunks.head->data);
            assert(node->data<chunks.tail->data);
            assert(chunks.size>=2);

            chunk *next = chunks.tail;
            chunk *prev = next->prev;

            while(prev)
            {
                if(prev->data<node->data&&node->data<next->data)
                {
                    // insertion
                    prev->next = node;
                    next->prev = node;
                    node->next = next;
                    node->prev = prev;
                    ++aliasing::_(chunks.size);
                    return;
                }
                next=prev;
                prev=prev->prev;
            }

            // never get here!
            cached.store( delete_chunk_data(node) );
            fatal_error("invalid system memory mapping in arena");
        }

        chunk * arena:: new_chunk()
        {
            //------------------------------------------------------------------
            //
            // ensure cached chunk
            //
            //------------------------------------------------------------------
            if(cached.size<=0)
            {
                prepare_page();
            }
            assert(cached.size>0);

            //------------------------------------------------------------------
            //
            // get a cached chunk, with no memory
            //
            //------------------------------------------------------------------
            chunk *ch = cached.query();
            try
            {
                //--------------------------------------------------------------
                // get memory
                //--------------------------------------------------------------
                static global &hmem = global::instance();
                void   *data = hmem.__calloc(1,chunk_size);

                //--------------------------------------------------------------
                //format the chunk
                //--------------------------------------------------------------
                new (ch) chunk(block_size,data,chunk_size);

                //--------------------------------------------------------------
                // update bookeeping
                //--------------------------------------------------------------
                available += ch->provided_number;
                insert_chunk(ch);
#if !defined(NDEBUG)
                for(const chunk *node=chunks.head;node->next;node=node->next)
                {
                    assert(node->data<node->next->data);
                }
#endif
                return ch;

            }
            catch(...)
            {
                cached.store(ch);
                throw;
            }
        }


    }
}

#include "y/exception.hpp"

namespace upsylon {

    namespace memory
    {
        ////////////////////////////////////////////////////////////////////////
        //
        // arena acquire
        //
        ////////////////////////////////////////////////////////////////////////
        void * arena:: acquire()
        {
            if(available>0)
            {
                //______________________________________________________________
                //
                // there exists some space somewhere
                //______________________________________________________________
                assert(acquiring); // always valid after first acquire
                if(acquiring->still_available<=0)
                {
                    chunk *lo = acquiring->prev;
                    chunk *up = acquiring->next;

                    //----------------------------------------------------------
                    // scan both direction
                    //----------------------------------------------------------
                    while(up&&lo)
                    {
                        if(lo->still_available)
                        {
                            acquiring = lo;
                            goto ACQUIRE;
                        }

                        if(up->still_available)
                        {
                            acquiring = up;
                            goto ACQUIRE;
                        }
                        lo = lo->prev;
                        up = up->next;
                    }

                    //----------------------------------------------------------
                    // scan remaining lo
                    //----------------------------------------------------------
                    while(lo)
                    {
                        if(lo->still_available)
                        {
                            acquiring=lo;
                            goto ACQUIRE;
                        }
                        lo=lo->prev;
                    }

                    //----------------------------------------------------------
                    // scan remaining up
                    //----------------------------------------------------------
                    while(up)
                    {
                        if(up->still_available)
                        {
                            acquiring=up;
                            goto ACQUIRE;
                        }
                        up=up->next;
                    }

                    throw exception("[arena.acquire(unexpected failure for block_size=%u)]", unsigned(block_size));
                }

            }
            else
            {
                acquiring = new_chunk();
                if(!releasing) releasing = acquiring;
            }

        ACQUIRE:
            //------------------------------------------------------------------
            //
            // at this point, everything is OK
            //
            //------------------------------------------------------------------
            assert(acquiring&&acquiring->still_available);
            assert(available>0);

            //------------------------------------------------------------------
            //
            // bookeeping
            //
            //------------------------------------------------------------------
            --available;
            if(empty==acquiring) empty=0;
            return acquiring->acquire();
        }

    }

}

namespace upsylon {

    namespace memory
    {

        bool  arena:: owns(const void *p) const throw()
        {
            assert(p);
            for(const chunk *ch=chunks.head;ch;ch=ch->next)
            {
                if(ch->owns(p))
                {
                    return true;
                }
            }
            return false;
        }

        ////////////////////////////////////////////////////////////////////////
        //
        // arena release
        //
        ////////////////////////////////////////////////////////////////////////
        void arena:: release(void *p) throw()
        {
            assert(p);
            assert(releasing);
            assert(acquiring);
            assert(global::exists());

            switch(releasing->whose(p))
            {
                case owned_by_this:
                    break;

                case owned_by_next:
                    // look up next
                    for(releasing=releasing->next;releasing;releasing=releasing->next)
                    {
                        if(releasing->owns(p)) break;
                    }
                    break;

                case owned_by_prev:
                    // look up prev
                    for(releasing=releasing->prev;releasing;releasing=releasing->prev)
                    {
                        if(releasing->owns(p)) break;
                    }
                    break;
            }
            assert((0!=releasing)    ||die("invalid address for arena"));
            assert((empty!=releasing)||die("invalid bookeeping")       );
            releasing->release(p);
            ++available;
            if(releasing->is_empty())
            {
                if(empty)
                {
                    //__________________________________________________________
                    //
                    // check highest memory
                    //__________________________________________________________
                    if(empty->data<releasing->data)
                    {
                        cswap(releasing,empty);
                    }

                    //__________________________________________________________
                    //
                    // remove empty, reinitialize acquiring if necessary
                    //__________________________________________________________
                    if(acquiring==empty)
                    {
                        acquiring = releasing;
                    }
                    //__________________________________________________________
                    //
                    // bookeeping of memory
                    //__________________________________________________________
                    available -= empty->provided_number;   // bookeeping

                    //__________________________________________________________
                    //
                    // and release some system memory
                    //__________________________________________________________
                    cached.store( delete_chunk_data( chunks.unlink(empty) ) );
                }

                empty = releasing;
            }
        }
    }

}
