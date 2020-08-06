
#include "y/memory/small/arena.hpp"
#include "y/type/utils.hpp"
#include "y/code/base2.hpp"
#include "y/type/aliasing.hpp"
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
                    std::cerr << "[memory::small::arena] leak #[block_size=" << block_size << "] = " << leak  << std::endl;
                }

            }

            static inline
            size_t chunk_size_for(const size_t block_size,
                                  const size_t chunk_size)
            {
                assert(block_size>0);
                const size_t the_min_chunk_size = chunk::min_chunk_size_for(block_size); assert(is_a_power_of_two(the_min_chunk_size));
                const size_t the_max_chunk_size = chunk::max_chunk_size_for(block_size); assert(is_a_power_of_two(the_max_chunk_size));
                assert(is_a_power_of_two(vein::min_bytes));


                const size_t min_cs = max_of(the_min_chunk_size,vein::min_bytes);
                const size_t max_cs = max_of(the_max_chunk_size,min_cs);
                return clamp(min_cs,next_power_of_two(chunk_size),max_cs);
            }



            

            arena:: arena(const size_t   the_block_size,
                          const size_t   req_chunk_size,
                          zcache<chunk> &Z,
                          quarry        &Q) :
            acquiring(0),
            releasing(0),
            empty_one(0),
            available(0),
            chunks(),
            block_size( the_block_size ),
            chunk_size( chunk_size_for(block_size,req_chunk_size) ),
            next(0),
            prev(0),
            zchunks(Z),
            deposit(Q(chunk_size)),
            blocks_per_chunk(0),
            reserved(0)
            {
                empty_one = acquiring = releasing = create_chunk();
                aliasing::_(blocks_per_chunk) = acquiring->provided_number;
                
            }

            chunk * arena:: create_chunk()
            {

                //--------------------------------------------------------------
                // get an empty chunk
                //--------------------------------------------------------------
                chunk * curr = zchunks.zquery();
                
                //--------------------------------------------------------------
                // provide memory to this chunk
                //--------------------------------------------------------------
                try
                {
                    new (curr) chunk(block_size,deposit.query(),chunk_size);
                }
                catch(...)
                {
                    zchunks.zstore(curr);
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
                // release memory into cache
                //--------------------------------------------------------------
                deposit.store(p->data);

                //--------------------------------------------------------------
                // return to cache
                //--------------------------------------------------------------
                zchunks.zstore(p);
            }

        }
    }

}

namespace upsylon {

    namespace memory {

        namespace small {

#define Y_ARENA_LOOK_UP(ID,WHICH) if(ID->still_available>0) { acquiring=ID; goto UPDATE; } ID=ID->WHICH
#define Y_ARENA_LOOK_UP_LO() Y_ARENA_LOOK_UP(lo,prev)
#define Y_ARENA_LOOK_UP_HI() Y_ARENA_LOOK_UP(hi,next)

            void * arena:: acquire()
            {
                assert(acquiring);
                assert(releasing);
                assert(NULL==empty_one||empty_one->is_empty());

                if(available)
                {
                    //----------------------------------------------------------
                    // there is a block available somewhere
                    //----------------------------------------------------------
                    if(acquiring->still_available)
                    {
                        //------------------------------------------------------
                        // cached! do nothing
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
                            Y_ARENA_LOOK_UP_LO();
                            Y_ARENA_LOOK_UP_HI();
                        }

                        while(lo)
                        {
                            Y_ARENA_LOOK_UP_LO();
                        }

                        while(hi)
                        {
                            Y_ARENA_LOOK_UP_HI();
                        }
                        // never get here
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
                assert(NULL==empty_one||empty_one->is_empty());
                --available;
                return acquiring->acquire(block_size);
            }

        }

    }

}


namespace upsylon {

    namespace memory {

        namespace small {

            void arena::  releasing_at(const void *addr) throw()
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
            }

            
            void arena:: release(void *addr) throw()
            {
                assert(NULL!=addr);
                assert(NULL!=releasing);

                //--------------------------------------------------------------
                // locating data
                //--------------------------------------------------------------
                releasing_at(addr);

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

                        // release the extra chunk
                        delete_chunk( chunks.unlink(to_free) );
                    }
                }

                
            }

        }

    }

}

#include <cstring>
namespace upsylon {

    namespace memory {

        namespace small {

            bool arena:: compact(void * &addr) throw()
            {
                assert(addr!=NULL);
                assert(releasing!=NULL);

                if(available)
                {
                    //----------------------------------------------------------
                    //
                    // check if available lower memory
                    //
                    //----------------------------------------------------------
                    releasing_at(addr);
                    for(chunk *guess=chunks.head;guess!=releasing;guess=guess->next)
                    {
                        if(guess->still_available>0)
                        {
                            //--------------------------------------------------
                            // get a no-throw block
                            //--------------------------------------------------
                            void *new_addr = (acquiring = guess) ->acquire(block_size);
                            assert(new_addr<addr);
                            
                            //--------------------------------------------------
                            // update status
                            //--------------------------------------------------
                            --available;
                            if(empty_one==acquiring) empty_one=0;

                            //--------------------------------------------------
                            // move memory
                            //--------------------------------------------------
                            memcpy(new_addr,addr,block_size);

                            //--------------------------------------------------
                            // release old addr
                            //--------------------------------------------------
                            release(addr);
                            addr = new_addr;
                            return true;
                        }
                    }
                    
                    //----------------------------------------------------------
                    //
                    // not found
                    //
                    //----------------------------------------------------------
                    return false;
                }
                else
                {
                    //----------------------------------------------------------
                    //
                    // no more available memory at all
                    //
                    //----------------------------------------------------------
                    return false;
                }
            }

        }

    }

}

#include <iostream>
namespace upsylon {

    namespace memory {

        namespace small {

            std::ostream & operator<<( std::ostream &os, const arena &a)
            {
                os << "\t<arena block_size=" << a.block_size << " chunk_size=" << a.chunk_size << "  blocks_per_chunk=" << a.blocks_per_chunk << ">" << std::endl;
                os << "\t  <chunks used=" << a.chunks.size << ">" << std::endl;
                size_t j=0;
                bool   r=false;
                for(const chunk *ch = a.chunks.head;ch;ch=ch->next)
                {
                    os << ' ' << *ch;
                    if( 0 == (++j%4) )
                    {
                        os << std::endl;
                        r = true;
                    }
                    else
                    {
                        r = false;
                    }
                }
                if(!r) os << std::endl;
                os << "\t  <chunks/>" << std::endl;
                os << "\t<arena/>";
                return os;
            }

        }

    }

}
