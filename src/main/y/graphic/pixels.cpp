
#include "y/graphic/pixels.hpp"
#include "y/code/base2.hpp"
#include "y/exception.hpp"
#include "y/type/block/zset.hpp"

namespace upsylon
{
    namespace graphic
    {

        pixels:: ~pixels() throw()
        {
            object::dyadic_release(block_addr,block_exp2);
            _bzset(block_addr);
            _bzset(block_size);
            _bzset(block_exp2);
        }


        static inline
        size_t block_size_for(const size_t required)
        {
            if(required>base2<size_t>::max_power_of_two)
            {
                throw exception("too many required pixels!");
            }
            return next_power_of_two(required);
        }

        pixels:: pixels(const size_t required) :
        object(),
        counted(),
        block_size( block_size_for(required) ),
        block_exp2( integer_log2(block_size) ),
        block_addr( object::dyadic_acquire(block_exp2) )
        {

        }

    }

}

