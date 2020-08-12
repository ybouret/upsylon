
#include "y/memory/tight/blocks.hpp"
#include "y/type/utils.hpp"
#include "y/code/base2.hpp"
#include "y/memory/allocator/global.hpp"
#include "y/type/self-destruct.hpp"
#include "y/type/block/zset.hpp"
#include "y/os/run-time-log.hpp"
#include <cstdlib>

namespace upsylon {

    namespace memory {

        namespace tight {

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
                        zArenas.zstore(a);
                    }
                }

                slots_vein.release(slot);
                slot=0;
                acquiring=releasing=0;
                _bzset(chunk_size);
                _bzset(slots_size);
                _bzset(limit_size);
            }



            static inline void * acquire_slots_with( vein &v )
            {
                return v.acquire();
            }

            blocks:: blocks(const size_t usr_chunk_size,
                            const size_t usr_limit_size,
                            quarry      &usr_sys_quarry) :
            oversized(0),
            chunk_size( next_power_of_two( max_of(usr_chunk_size,usr_limit_size,min_chunk_size) ) ),
            limit_size(usr_limit_size),
            slots_size(most_significant_bit(chunk_size/sizeof(blocks::slot_type))),
            slots_mask(slots_size-1 ),
            slots_vein( usr_sys_quarry(chunk_size) ),
            slot( static_cast<slot_type *>(acquire_slots_with(slots_vein) ) ),
            acquiring(0),
            releasing(0),
            sharedQ(usr_sys_quarry),
            zChunks(chunk_size,sharedQ),
            zArenas(chunk_size,sharedQ)
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

            arena * blocks:: query(const size_t block_size)
            {
                assert(block_size>0);
                assert(block_size<=limit_size);

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
                    return acquiring;
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
                            return (acquiring=entry.move_to_front(a));
                        }
                    }

                    //------------------------------------------------------
                    // create a new arena
                    //------------------------------------------------------
                    arena *a = zArenas.zquery();
                    try {
                        new (a) arena(block_size,chunk_size,zChunks,sharedQ);
                    }
                    catch(...)
                    {
                        zArenas.zstore(a);
                        throw;
                    }

                    //------------------------------------------------------
                    // return block
                    //------------------------------------------------------
                    return (acquiring=entry.push_front(a) );
                }
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
                    static global &mgr  = global::instance();
                    void          *addr =  mgr.__calloc(1,block_size);
                    oversized += block_size;
                    return addr;
                }
                else
                {
                    return query(block_size)->acquire();
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
                    // switch to global memory
                    //
                    //----------------------------------------------------------
                    assert(addr!=0);
                    assert(block_size<=oversized);
                    static global &mgr = global::location();
                    mgr.__free(addr,block_size);
                    oversized-=block_size;
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
                        rtl(run_time_error) << "[tight::blocks] invalid address to release" << std::endl;
                        exit(-1);
                    }
                }
                
            }

            bool blocks:: try_compact(void * &addr, const size_t block_size) throw()
            {
                slot_type &entry = slot[block_size&slots_mask];
                for(arena *a=entry.head;a;a=a->next)
                {
                    if(block_size==a->block_size)
                    {
                        return a->compact(addr);
                    }
                }
                return false; // shouldn't happen
            }



        }

    }

}


namespace upsylon {

    namespace memory {

        namespace tight {

            std::ostream & operator<<( std::ostream &os, const blocks &b)
            {
                os << "<blocks chunk_size=" << b.chunk_size << " limit_size=" << b.limit_size << " #slots=" << b.slots_size << " >" << std::endl;
                os << "\t<oversized bytes=" << b.oversized << "/>" << std::endl;
                for(size_t i=0;i<b.slots_size;++i)
                {
                    const blocks::slot_type &entry = b.slot[i];
                    if(entry.size)
                    {
                        os << "\t<slot[" << i << "] #arena=" << entry.size << " {";
                        for(const arena *a=entry.head;a;a=a->next)
                        {
                            os << ' ' << a->block_size;
                        }
                        os << " } >" << std::endl;

                        for(const arena *a=entry.head;a;a=a->next)
                        {
                            os << *a << std::endl;
                        }

                        os << "\t<slot[" << i << "]/>" << std::endl;
                    }
                }
                os << "<blocks/>" << std::endl;
                return os;
            }
            
        }

    }

}

#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon {

    namespace memory {

        namespace tight {

            static const char fn[] = "tight::blocks";

            arena & blocks:: operator[](const size_t block_size)
            {
                if(block_size<=0||block_size>limit_size)
                    throw libc::exception(EDOM,"%s(block_size=%lu not in [1:%lu])",fn,(unsigned long)block_size, (unsigned long)limit_size);
                return *query(block_size);
            }

            const arena & blocks:: operator[](const size_t block_size) const
            {
                const slot_type &entry = slot[block_size&slots_mask];
                for(const arena *a=entry.head;a;a=a->next)
                {
                    if(block_size==a->block_size) return *a;
                }
                throw libc::exception(EINVAL,"no %s[%lu]",fn,(unsigned long)block_size);
            }
        }
    }
}
