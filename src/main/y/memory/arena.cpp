#include "y/memory/arena.hpp"
#include "y/type/utils.hpp"
#include "y/memory/global.hpp"
#include "y/os/error.hpp"

#include <iostream>

namespace upsylon {

    namespace memory
    {

        arena:: ~arena() throw()
        {
            if(mstore.size)
            {
                assert(global::exists());
                global &hmem = global::location();
                while(mstore.size)
                {
                    hmem.__free(mstore.query(),chunk_size);
                }
                cached.reset();
                for(const mchunk *node=chunks.head;node;node=node->next)
                {
                    if( !node->is_empty() )
                    {
                        std::cerr << "[memory.chunk] still #allocated=" << node->allocated() << std::endl;
                    }
                    hmem.__free(node->data,io::delta(node->data,node->last));
                }
                chunks.reset();
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
        chunks(),
        cached(),
        mstore(),
        chunks_per_mblock(chunk_size/sizeof(mchunk)-1)
        {
            std::cerr << "[memory.arena] block_size        = " << block_size << std::endl;
            std::cerr << "[memory.arena] chunk_size        = " << chunk_size << std::endl;
            std::cerr << "[memory.arena] sizeof(mchunk)    = " << sizeof(mchunk) << std::endl;
            std::cerr << "[memory.arena] chunks_per_mblock = " << chunks_per_mblock << std::endl;
            assert(block_size>0);
            assert(chunks_per_mblock>0);

        }

        void arena:: new_mblock()
        {
            global &hmem = global::instance();                  //!< global allocator
            void   *addr = hmem.__calloc(1,chunk_size);         //!< get chunk_size
            mblock *blk  = static_cast<mblock *>(addr);         //!< convert into mblock
            mstore.store(blk);                                  //!< kept into blocks
            mchunk *ch = io::cast<mchunk>(addr,sizeof(mchunk)); //!< get first mchunk

            // store all memory in cached
            for(size_t i=0;i<chunks_per_mblock;++i)
            {
                cached.store( ch+i );
            }
            std::cerr << "now using #mblock=" << mstore.size << std::endl;
        }

        void arena:: load_new_chunk(mchunk *node) throw()
        {
            Y_CORE_CHECK_LIST_NODE(node);


            if(chunks.size<=0)
            {
                std::cerr << "+chunk@first" << std::endl;
                chunks.push_back(node);
                return;
            }

            assert(chunks.size>0);

            if(node->data>chunks.tail->data)
            {
                std::cerr << "+chunk>last" << std::endl;
                chunks.push_back(node);
                return;
            }

            if(node->data<chunks.head->data)
            {
                std::cerr << "+chunk<head" << std::endl;
                chunks.push_front(node);
                return;
            }
            assert(node->data>chunks.head->data);
            assert(node->data<chunks.tail->data);
            assert(chunks.size>=2);

            mchunk *next = chunks.tail;
            mchunk *prev = next->prev;

            while(prev)
            {
                if(prev->data<node->data&&node->data<next->data)
                {
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

        mchunk * arena:: new_mchunk()
        {
            // ensure cached mchunk
            if(cached.size<=0)
            {
                new_mblock();
            }
            assert(cached.size>0);

            // get a cached mchunk, with no memory
            mchunk *ch = cached.query();
            try
            {
                // get memory
                global &hmem = global::instance();
                void   *data = hmem.__calloc(1,chunk_size);

                //format the chunk
                new (ch) mchunk(block_size,data,chunk_size);

                // update bookeeping
                available += ch->provided_number;
                std::cerr << "available=" << available << std::endl;
                load_new_chunk(ch);
#if !defined(NDEBUG)
                for(const mchunk *node=chunks.head;node->next;node=node->next)
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
                    mchunk *lo = acquiring->prev;
                    mchunk *up = acquiring->next;

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
                acquiring = new_mchunk();
                if(!releasing) releasing = acquiring;
            }

        ACQUIRE:
            // at this point, everything is OK
            assert(acquiring&&acquiring->still_available);
            assert(available>0);
            --available;
            return acquiring->acquire();
        }

    }

}

namespace upsylon {

    namespace memory
    {
        void arena:: release(void *p) throw()
        {
            assert(p);
            assert(releasing);
            switch(releasing->owned_by(p))
            {
                case mchunk::owned_by_this:
                    break;

                case mchunk::owned_by_next:
                    // look up next
                    for(releasing=releasing->next;releasing;releasing=releasing->next)
                    {
                        if(releasing->owns(p)) break;
                    }
                    break;

                case mchunk::owned_by_prev:
                    // look up prev
                    for(releasing=releasing->prev;releasing;releasing=releasing->prev)
                    {
                        if(releasing->owns(p)) break;
                    }
                    break;
            }
            assert(releasing||die("invalid address for arena"));
            releasing->release(p);
            ++available;
            if(releasing->is_empty())
            {
            }
        }
    }

}
