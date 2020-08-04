
#include "y/memory/small/blocks.hpp"
#include "y/type/utils.hpp"
#include "y/memory/allocator/global.hpp"
#include "y/type/self-destruct.hpp"
#include "y/type/block/zset.hpp"

namespace upsylon {

    namespace memory {

        namespace small {

            blocks:: ~blocks() throw()
            {
                size_t i=slots_size;
                while(i-- >0)
                {
                    slot_type &entry = slot[i];
                    while(entry.size)
                    {
                        arena *a = entry.pop_back();
                        self_destruct(*a);
                        arenas.store_nil(a);
                    }
                }

                global::location().__free(slot,chunk_size);
                slot=0;
                acquiring=releasing=0;
                _bzset(chunk_size);
                _bzset(slots_size);
                _bzset(limit_size);

            }



            blocks:: blocks(const size_t usr_chunk_size,
                            const size_t usr_limit_size) :
            chunk_size( next_power_of_two( max_of(usr_chunk_size,usr_limit_size,min_chunk_size) ) ),
            slots_size(most_significant_bit_mask(chunk_size/sizeof(blocks::slot_type))),
            slots_mask(slots_size-1 ),
            limit_size(usr_limit_size),
            slot( static_cast<slot_type *>(global::instance().__calloc(1,chunk_size) ) ),
            acquiring(0),
            releasing(0),
            chunks(chunk_size),
            arenas(chunk_size)
            {
                for(size_t i=0;i<slots_size;++i)
                {
                    new (&slot[i]) slot_type();
                }
            }

            size_t blocks:: load_factor() const throw()
            {
                return limit_size/slots_size;
            }

            void * blocks:: acquire(const size_t block_size)
            {
                if(block_size<=0)
                {
                    //----------------------------------------------------------
                    //
                    // convention: returns 0
                    //
                    //----------------------------------------------------------
                    return 0;
                }
                else if(block_size>limit_size)
                {
                    //----------------------------------------------------------
                    //
                    // switch to global
                    //
                    //----------------------------------------------------------
                    static global &mgr = global::instance();
                    return mgr.__calloc(1,block_size);
                }
                else
                {
                    //----------------------------------------------------------
                    //
                    // look up for acquiring
                    //
                    //----------------------------------------------------------
                    if(acquiring&&acquiring->block_size==block_size)
                    {
                        //------------------------------------------------------
                        // cached!
                        //------------------------------------------------------
                        return acquiring->acquire();
                    }
                    else
                    {
                        //------------------------------------------------------
                        // look up!
                        //------------------------------------------------------
                        slot_type &entry = slot[block_size&slots_mask];
                        for(arena *a=entry.head;a;a=a->next)
                        {
                            if(block_size==a->block_size)
                            {
                                // Found!
                                return (acquiring=entry.move_to_front(a))->acquire();
                            }
                        }

                        //------------------------------------------------------
                        // create a new arena
                        //------------------------------------------------------
                        arena *a = arenas.query_nil();
                        try {
                            new (a) arena(block_size,chunk_size,chunks);
                        }
                        catch(...)
                        {
                            arenas.store_nil(a);
                            throw;
                        }

                        //------------------------------------------------------
                        // return block
                        //------------------------------------------------------
                        return (acquiring=entry.push_front(a) )->acquire();
                    }
                }
            }

            void  blocks:: release(void *addr, const size_t block_size) throw()
            {
                if(block_size<=0)
                {
                    //----------------------------------------------------------
                    //
                    // convention: do nothing
                    //
                    //----------------------------------------------------------
                    assert(0==addr);
                }
                else if(block_size>limit_size)
                {
                    //----------------------------------------------------------
                    //
                    // forward to global memory
                    //
                    //----------------------------------------------------------
                    assert(addr!=0);
                    static global &mgr = global::location();
                    mgr.__free(addr,block_size);
                }
                else
                {
                    //----------------------------------------------------------
                    //
                    // look up
                    //
                    //----------------------------------------------------------
                    assert(addr!=0);
                    if( releasing && block_size == releasing->block_size )
                    {
                        // cached
                        releasing->release(addr);
                    }
                    else
                    {
                        // look up
                        slot_type &entry = slot[block_size&slots_mask];
                        for(arena *a=entry.head;a;a=a->next)
                        {
                            if(block_size==a->block_size)
                            {
                                ( releasing = entry.move_to_front(a) )->release(addr);
                                return;
                            }
                        }

                        // should never get here
                        assert( die("invalid address in small.blocks.release") );
                    }
                }
                
            }



        }

    }

}

