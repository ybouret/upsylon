
#include "y/memory/dblock.hpp"
#include "y/code/base2.hpp"
#include "y/type/utils.hpp"
#include "y/type/block/zset.hpp"
#include "y/object.hpp"
#include <cstring>

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


        size_t dblock:: regularize(const size_t block_exp2) throw()
        {
            return clamp<size_t>( ilog2_of<void*>::value, block_exp2, base2<size_t>::max_shift );
        }


        

        dblock:: dblock(const size_t block_exp2) :
        exp2( regularize(block_exp2) ),
        size( size_t(1) << exp2 ),
        addr( static_cast<uint8_t *>(object::dyadic_acquire(exp2)) ),
        next(0),
        prev(0),
        origin(0),
        source(0)
        {
        }


        dblock:: dblock(const size_t block_exp2,
                        const size_t block_size,
                        dblocks    * block_org,
                        core_list  * block_src) :
        exp2(block_exp2),
        size(block_size),
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

        void dblock:: zap(dblock *pointee) throw()
        {
            assert(pointee!=NULL);
            if(pointee->origin)
            {
                dblocks &blocks = *(pointee->origin);
                Y_LOCK(blocks.sync);
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
        }
        
        void dblock:: ldz() throw()
        {
            memset(addr,0,size);
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
            dblock::zap(pointee);
            pointee=0;
        }

    }

}


namespace upsylon
{
    namespace memory
    {

        dblock:: list:: list() throw() : core_list()
        {
        }

        dblock:: list:: ~list() throw()
        {
            while(size)
            {
                zap( pop_back() );
            }
        }

    }

}

#include <iomanip>

namespace upsylon
{
    namespace memory
    {

        dblocks:: ~dblocks() throw()
        {
            Y_LOCK(sync);
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
        sync(shared_access)
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

#define Y_DBLOCKS_QUERY_PROLOG() \
block_exp2              = dblock::regularize(block_exp2);\
const size_t block_size = size_t(1) << block_exp2;\
slot_type   &s          = slot[block_exp2]

#define Y_DBLOCKS_NEW() new dblock(block_exp2,block_size,this,&s)

        dblock * dblocks:: query(size_t block_exp2)
        {
            Y_LOCK(sync);
            Y_DBLOCKS_QUERY_PROLOG();
            if(s.size)
            {
                dblock *blk = s.pop_back(); assert(blk->origin==this); assert(&s==blk->source);
                return  blk;
            }
            else
            {
                return Y_DBLOCKS_NEW();
            }
        }

        void dblocks:: reserve(size_t n, size_t block_exp2)
        {
            Y_LOCK(sync);
            Y_DBLOCKS_QUERY_PROLOG();
            while(n-- > 0)
            {
                s.push_back( Y_DBLOCKS_NEW() );
            }
        }

        
        std::ostream & dblocks:: display(std::ostream &os) const
        {
            static const size_t one = 1;
            os << "<dblocks>" << std::endl;
            for(size_t i=0;i<slots;++i)
            {
                const slot_type &s = slot[i];
                if(s.size)
                {
                    os << '\t' <<  '[' << std::setw(12) << (one<<i) << ']' << " : " << s.size << std::endl;
                }
            }
            os << "<dblocks/>" ;
            return os;
        }

        std::ostream &   operator<<(std::ostream &os, const dblocks &db)
        {
            return db.display(os);
        }


    }

}

