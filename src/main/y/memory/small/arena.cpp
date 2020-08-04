
#include "y/memory/small/arena.hpp"
#include "y/type/utils.hpp"
#include "y/memory/allocator/global.hpp"
#include <iostream>

namespace upsylon {

    namespace memory {

        namespace small {

            arena:: ~arena() throw()
            {
                size_t leak = 0;
                while(chunks.size)
                {
                    chunk       *p = chunks.pop_back();
                    const size_t n = p->allocated();
                    if(n>0)
                    {
                        leak      += n;
                        available += n;
                    }
                    delete_chunk(p);
                }
                if(leak>0)
                {
                    std::cerr << "[memory::small::arena] leak #block_size=" << block_size << " = " << leak  << std::endl;
                }

            }

            static inline
            size_t chunk_size_for(const size_t block_size,
                                  const size_t chunk_size)
            {
                assert(block_size>0);
                const size_t min_cs = chunk::min_chunk_size_for(block_size);
                const size_t max_cs = chunk::max_chunk_size_for(block_size);
                return clamp(min_cs,next_power_of_two(chunk_size),max_cs);
            }

            size_t arena:: blocks_per_chunk() const throw()
            {
                assert(acquiring);
                return acquiring->provided_number;
            }


            arena:: arena(const size_t usr_block_size,
                              const size_t usr_chunk_size,
                              zchunks       &cache) :
            acquiring(0),
            releasing(0),
            empty_one(0),
            available(0),
            chunks(),
            shared( &cache ),
            block_size( usr_block_size ),
            chunk_size( chunk_size_for(block_size,usr_chunk_size) )
            {
                empty_one = acquiring = releasing = create_chunk();
            }

            chunk * arena:: create_chunk()
            {
                static global &mgr = mgr.instance();
                assert(shared);

                //--------------------------------------------------------------
                // get an empty piece
                //--------------------------------------------------------------
                chunk * curr = shared->query_nil();

                //--------------------------------------------------------------
                // provide memory to this piece
                //--------------------------------------------------------------
                try
                {
                    new (curr) chunk(block_size,mgr.__calloc(1,chunk_size),chunk_size);
                }
                catch(...)
                {
                    shared->store_nil(curr);
                    throw;
                }

                //--------------------------------------------------------------
                // put in position
                //--------------------------------------------------------------
                (void) chunks.push_back(curr);
                while(curr->prev&&curr->data<=curr->prev->data)
                {
                    chunks.towards_head(curr);
                }

                //--------------------------------------------------------------
                // update available
                //--------------------------------------------------------------
                available += curr->provided_number;
                return curr;
            }



            void  arena:: delete_chunk(chunk *p) throw()
            {
                static global &mgr = global::location();

                assert(p);
                assert(0==p->next);
                assert(0==p->prev);
                assert(p->data!=NULL);
                assert(available>=p->provided_number);

                //--------------------------------------------------------------
                // update available
                //--------------------------------------------------------------
                available -= p->provided_number;

                //--------------------------------------------------------------
                // release memory
                //--------------------------------------------------------------
                mgr.__free(p->data,chunk_size);

                //--------------------------------------------------------------
                // return to cache
                //--------------------------------------------------------------
                shared->store_nil(p);
            }

        }
    }

}

namespace upsylon {

    namespace memory {

        namespace small {

            void * arena:: acquire()
            {
                assert(acquiring);
                assert(releasing);

                if(available)
                {
                    //----------------------------------------------------------
                    // there is a still available piece somewhere!
                    //----------------------------------------------------------
                    if(acquiring->still_available)
                    {
                        //------------------------------------------------------
                        // cached!
                        //------------------------------------------------------
                    }
                    else
                    {
                        //------------------------------------------------------
                        // local interleaved search
                        //------------------------------------------------------
                        chunk *lo = acquiring->prev;
                        chunk *hi = acquiring->next;
                        while(lo&&hi)
                        {
                            if(lo->still_available>0)
                            {
                                acquiring = lo; goto UPDATE;
                            }
                            lo=lo->prev;
                            if(hi->still_available>0)
                            {
                                acquiring = hi; goto UPDATE;
                            }
                            hi=hi->next;
                        }

                        while(lo)
                        {
                            if(lo->still_available>0)
                            {
                                acquiring = lo; goto UPDATE;
                            }
                            lo=lo->prev;
                        }

                        while(hi)
                        {
                            if(hi->still_available>0)
                            {
                                acquiring = hi; goto UPDATE;
                            }
                            hi=hi->next;
                        }
                    }
                UPDATE:
                    assert(acquiring);
                    assert(acquiring->still_available);

                    //----------------------------------------------------------
                    // check empty_one
                    //----------------------------------------------------------
                    if(empty_one==acquiring)
                    {
                        empty_one=0;
                    }
                }
                else
                {
                    //----------------------------------------------------------
                    // need a new piece
                    //----------------------------------------------------------
                    assert(0==empty_one);
                    chunk *p  = create_chunk();
                    acquiring = p;
                }


                assert(acquiring);
                assert(acquiring->still_available);
                assert(!(empty_one&&acquiring==empty_one));
                --available;
                return acquiring->acquire(block_size);
            }

        }

    }

}


namespace upsylon {

    namespace memory {

        namespace small {

            void arena:: release(void *addr) throw()
            {
                assert(NULL!=addr);
                assert(NULL!=releasing);

                //--------------------------------------------------------------
                // locating data
                //--------------------------------------------------------------
                switch (releasing->whose(addr))
                {
                    case owned_by_this: break;
                    case owned_by_prev: do { releasing=releasing->prev; assert(releasing); } while( !releasing->owns(addr) ); break;
                    case owned_by_next: do { releasing=releasing->next; assert(releasing); } while( !releasing->owns(addr) ); break;
                }

                //--------------------------------------------------------------
                // release block and update available
                //--------------------------------------------------------------
                assert(releasing->owns(addr));
                assert(empty_one!=releasing);

                releasing->release(addr,block_size);
                ++available;

                //--------------------------------------------------------------
                // check memory status
                //--------------------------------------------------------------
                if(releasing->is_empty())
                {
                    assert(releasing->allocated()==0);
                    if(0==empty_one)
                    {
                        //------------------------------------------------------
                        // initialize empty_one
                        //------------------------------------------------------
                        empty_one = releasing;
                    }
                    else
                    {
                        //------------------------------------------------------
                        // two empty pieces: free the piece with higher memory
                        //------------------------------------------------------
                        chunk *to_free = releasing;
                        chunk *to_keep = empty_one;
                        if(to_free->data<to_keep->data)
                        {
                            cswap(to_keep,to_free);
                        }
                        assert(to_keep->data<to_free->data);

                        // update status
                        empty_one = to_keep;
                        if(acquiring==to_free)
                        {
                            acquiring = to_keep;
                        }
                        releasing = acquiring;
                        
                        delete_chunk( chunks.unlink(to_free) );

                    }
                }

                
            }

        }

    }

}


