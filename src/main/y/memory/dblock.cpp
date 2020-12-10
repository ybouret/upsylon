
#include "y/memory/dblock.hpp"
#include "y/code/base2.hpp"
#include "y/type/utils.hpp"
#include "y/type/block/zset.hpp"
#include "y/object.hpp"

namespace upsylon
{
    namespace memory
    {

        dblock:: ~dblock() throw()
        {
            assert(0==origin);
            assert(0==source);
            object::dyadic_release(addr,exp2);
            _bzset(addr);
            _bzset(size);
            _bzset(exp2);
        }


        static inline
        size_t dblock_size(const size_t bs)
        {
            return next_power_of_two( clamp(sizeof(void*),bs,base2<size_t>::max_power_of_two) );
        }

        dblock:: dblock(const size_t bs) :
        size( dblock_size(bs) ),
        exp2( base2<size_t>::log2_of(size) ),
        addr( static_cast<uint8_t *>(object::dyadic_acquire(exp2)) ),
        next(0),
        prev(0),
        origin(0),
        source(0)
        {
        }


        dblock:: dblock(const size_t block_size,
                        const size_t block_exp2,
                        dblocks    * block_org,
                        list_type  * block_src) :
        size(block_size),
        exp2(block_exp2),
        addr( static_cast<uint8_t *>(object::dyadic_acquire(exp2)) ),
        next(0),
        prev(0),
        origin(block_org),
        source(block_src)
        {
            assert(origin);
            assert(source);

        }


        uint8_t * dblock:: operator*()       throw()
        {
            assert(addr);
            return addr;
        }

        const uint8_t * dblock:: operator*() const throw()
        {
            assert(addr);
            return addr;
        }
        
    }
}


namespace upsylon
{
    namespace memory
    {
        dblock:: pointer:: pointer( dblock *blk ) throw() : ptr<dblock>(blk)
        {
            assert(pointee!=NULL);
        }

        dblock:: pointer:: ~pointer() throw()
        {
            assert(pointee!=NULL);
            if(pointee->origin)
            {
                dblocks &blocks = *(pointee->origin);
                Y_LOCK(blocks.access);
                assert(pointee->source!=NULL);
                assert(pointee->exp2<blocks.slots);
                assert(&blocks[pointee->exp2]==pointee->source);
                pointee->source->push_back(pointee);
            }
            else
            {
                assert(0==pointee->source);
                delete pointee;
            }
            pointee=0;
        }

    }

}


namespace upsylon
{
    namespace memory
    {

        dblocks:: ~dblocks() throw()
        {
            Y_LOCK(access);
            for(size_t i=0;i<slots;++i)
            {
                slot_type &s = slot[i];
                while(s.size)
                {
                    dblock *blk = s.pop_back();
                    blk->origin = 0;
                    blk->source = 0;
                    delete blk;
                }
                s.~slot_type();
            }

            object::dyadic_release(slot,mem_exp2);
            _bzset(slot);
            _bzset(slots);
            _bzset(mem_size);
            _bzset(mem_exp2);

        }


        dblocks:: dblocks(lockable &shared_access) :
        slots(    base2<size_t>::max_slots  ),
        mem_size( sizeof(slot_type) * slots ),
        mem_exp2( base2<size_t>::log2_of(mem_size) ),
        slot( static_cast<slot_type*>( object::dyadic_acquire(mem_exp2) )),
        access(shared_access)
        {
            for(size_t i=0;i<slots;++i)
            {
                new (slot+i) slot_type();
            }
        }



        const dblocks::slot_type & dblocks:: operator[](const size_t islot) const throw()
        {
            assert(islot<slots);
            return slot[islot];
        }

        dblock * dblocks:: query(const size_t bs)
        {
            Y_LOCK(access);
            const size_t block_size = dblock_size(bs);
            const size_t block_exp2 = base2<size_t>::log2_of(block_size); assert(block_exp2<slots);
            slot_type   &s          = slot[block_exp2];
            if(s.size)
            {
                dblock *blk = s.pop_back(); assert(blk->origin==this); assert(&s==blk->source);
                return  blk;
            }
            else
            {
                return new dblock(block_size,block_exp2,this,&s);
            }

        }


    }

}

