#include "y/memory/arena.hpp"
#include "y/type/utils.hpp"
#include "y/memory/global.hpp"
#include "y/os/error.hpp"

#include <iostream>

namespace upsylon {

    namespace memory
    {

        ////////////////////////////////////////////////////////////////////////
        //
        // arena setup
        //
        ////////////////////////////////////////////////////////////////////////

        arena:: ~arena() throw()
        {
            if(cstore.size)
            {
                assert(global::exists());

                //______________________________________________________________
                //
                // hard reset cached, for these are only memory locations
                //______________________________________________________________
                cached.reset();

                //______________________________________________________________
                //
                // clean allocated chunks
                //______________________________________________________________
                global &hmem = global::location();
                for(const chunk *node=chunks.head;node;node=node->next)
                {
                    if( !node->is_empty() )
                    {
                        std::cerr << "[memory.chunk] still #allocated=" << node->allocated() << std::endl;
                    }
                    hmem.__free(node->data,io::delta(node->data,node->last));
                }
                // then hard reset
                chunks.reset();

                //______________________________________________________________
                //
                // remove effective storage
                //______________________________________________________________
                while(cstore.size)
                {
                    hmem.__free(cstore.query(),chunk_size);
                }

            }
            else
            {
                assert(0==cached.size);
                assert(0==chunks.size);
            }
        }

        arena:: arena(const size_t the_block_size,
                      const size_t the_chunk_size) throw() :
        block_size(the_block_size),
        chunk_size( max_of(the_chunk_size,min_chunk_size) ),
        acquiring(0),
        releasing(0),
        available(0),
        empty(0),
        chunks(),
        cached(),
        cstore(),
        chunks_per_block(chunk_size/sizeof(chunk)-1)
        {
            assert(block_size>0);
            assert(chunks_per_block>0);
        }

        void arena:: new_block()
        {
            global &hmem = global::instance();                  //!< global allocator
            void   *addr = hmem.__calloc(1,chunk_size);         //!< get chunk_size
            block  *blk  = static_cast<block *>(addr);          //!< convert into block
            cstore.store(blk);                                  //!< kept into blocks
            chunk *ch = io::cast<chunk>(addr,sizeof(chunk));    //!< get first chunk address

            // store all memory in cached list
            for(size_t i=0;i<chunks_per_block;++i)
            {
                cached.store( ch+i );
            }
        }

        void arena:: load_new_chunk(chunk *node) throw()
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
                    ++(chunks.size);
                    return;
                }
                next=prev;
                prev=prev->prev;
            }

            // never get here!
            fatal_error("invalid system memory mapping in arena");
        }

        chunk * arena:: new_chunk()
        {
            // ensure cached mchunk
            if(cached.size<=0)
            {
                new_block();
            }
            assert(cached.size>0);

            // get a cached chunk, with no memory
            chunk *ch = cached.query();
            try
            {
                // get memory
                global &hmem = global::instance();
                void   *data = hmem.__calloc(1,chunk_size);

                //format the chunk
                new (ch) chunk(block_size,data,chunk_size);

                // update bookeeping
                available += ch->provided_number;
                load_new_chunk(ch);
#if !defined(NDEBUG)
                for(const chunk *node=chunks.head;node->next;node=node->next)
                {
                    assert(node->data<node->next->data);
                }
#endif

            }
            catch(...)
            {
                cached.store(ch);
                throw;
            }
            return ch;
        }


    }
}

#include "y/exceptions.hpp"
#include <cerrno>

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

                    // scan both direction
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

                    // scan remaining lo
                    while(lo)
                    {
                        if(lo->still_available)
                        {
                            acquiring=lo;
                            goto ACQUIRE;
                        }
                        lo=lo->prev;
                    }

                    // scan remaining up
                    while(up)
                    {
                        if(up->still_available)
                        {
                            acquiring=up;
                            goto ACQUIRE;
                        }
                        up=up->next;
                    }

                    throw libc::exception(ENOMEM,"unexpected failure in arena.acquire(block_size=%u)", unsigned(block_size));
                }

            }
            else
            {
                acquiring = new_chunk();
                if(!releasing) releasing = acquiring;
            }

        ACQUIRE:
            // at this point, everything is OK
            assert(acquiring&&acquiring->still_available);
            assert(available>0);

            // bookeeping
            --available;
            if(empty==acquiring) empty=0;
            return acquiring->acquire();
        }

    }

}

namespace upsylon {

    namespace memory
    {
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
            switch(releasing->whose(p))
            {
                case chunk::owned_by_this:
                    break;

                case chunk::owned_by_next:
                    // look up next
                    for(releasing=releasing->next;releasing;releasing=releasing->next)
                    {
                        if(releasing->owns(p)) break;
                    }
                    break;

                case chunk::owned_by_prev:
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
                    //std::cerr << "two empty chunks!" << std::endl;
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
                    available -= empty->provided_number;   // bookeeping
                    cached.store( chunks.unlink(empty) );  // clear chunk
                    global::location().__free(empty->data,io::delta(empty->data,empty->last));
                }

                empty = releasing;
            }
        }
    }

}
