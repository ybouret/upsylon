
#include "y/memory/small/theater.hpp"
#include "y/type/utils.hpp"
#include "y/memory/allocator/global.hpp"
#include "y/type/aliasing.hpp"
#include <iostream>

namespace upsylon {

    namespace memory {

        namespace small {

            theater:: ~theater() throw()
            {
                size_t leak = 0;
                while(pieces.size)
                {
                    piece       *p = pieces.pop_back();
                    const size_t n = p->allocated();
                    if(n>0)
                    {
                        leak += n;
                        aliasing::_(available) += n;
                    }
                    delete_piece(p);
                }
                if(leak>0)
                {
                    std::cerr << "[memory::small::theater] leak #block_size=" << block_size << " = " << leak  << std::endl;
                }

            }

            static inline
            size_t chunk_size_for(const size_t block_size,
                                  const size_t chunk_size)
            {
                assert(block_size>0);
                const size_t min_cs = piece::min_chunk_size_for(block_size);
                const size_t max_cs = piece::max_chunk_size_for(block_size);
                return clamp(min_cs,next_power_of_two(chunk_size),max_cs);
            }



            theater:: theater(const size_t usr_block_size,
                              const size_t usr_chunk_size,
                              pages &cache) :
            acquiring(0),
            releasing(0),
            empty_one(0),
            available(0),
            pieces(),
            shared( &cache ),
            block_size( usr_block_size ),
            chunk_size( chunk_size_for(block_size,usr_chunk_size) )
            {
                empty_one = acquiring = releasing = create_piece();
            }

            piece * theater:: create_piece()
            {
                static global &mgr = mgr.instance();
                assert(shared);

                // get an empty piece
                piece * curr = shared->query_nil();

                // provide memory
                try {
                    new (curr) piece(block_size,mgr.__calloc(1,chunk_size),chunk_size);
                }
                catch(...)
                {
                    shared->store_nil(curr);
                    throw;
                }

                // put in position
                (void) pieces.push_back(curr);
                while(curr->prev&&curr->data<=curr->prev->data)
                {
                    pieces.towards_head(curr);
                }

                // update available
                aliasing::_(available) += curr->provided_number;
                return curr;
            }



            void  theater:: delete_piece(piece *p) throw()
            {
                static global &mgr = global::location();

                assert(p);
                assert(0==p->next);
                assert(0==p->prev);
                assert(p->data!=NULL);
                assert(available>=p->provided_number);

                // update available
                aliasing::_(available) -= p->provided_number;

                // release memory
                mgr.__free(p->data,chunk_size);

                // return to cache
                shared->store_nil(p);
            }


            void * theater:: acquire()
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
                        piece *lo = acquiring->prev;
                        piece *hi = acquiring->next;
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

                    // check empty_one
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
                    piece *p  = create_piece();
                    acquiring = p;
                }


                assert(acquiring);
                assert(acquiring->still_available);
                assert(!(empty_one&&empty_one==acquiring));
                --aliasing::_(available);
                return acquiring->acquire(block_size);
            }

        }

    }

}


