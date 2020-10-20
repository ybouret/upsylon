
#include "y/memory/buffers.hpp"
#include "y/code/base2.hpp"
#include "y/object.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/block/zset.hpp"
namespace upsylon
{

    namespace memory
    {

        cblock:: ~cblock() throw()
        {
            object::dyadic_release(block_addr,block_exp2);
            _bzset(block_size);
            _bzset(block_exp2);
            _bzset(block_addr);
        }

        static inline size_t cblock_size(const size_t n) throw()
        {
            return n < sizeof(void*) ? sizeof(void*) : n;
        }

        cblock:: cblock(const size_t n) :
        block_size( next_power_of_two(cblock_size(n)) ),
        block_exp2( integer_log2(block_size)   ),
        block_addr( object::dyadic_acquire(block_exp2) )
        {

        }

        const void * cblock:: ro() const throw()
        {
            return block_addr;
        }

        size_t cblock:: length() const throw()
        {
            return block_size;
        }


    }

}

