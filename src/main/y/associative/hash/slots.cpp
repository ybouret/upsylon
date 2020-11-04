#include "y/associative/hash/slots.hpp"
#include "y/type/block/zset.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/utils.hpp"
#include "y/code/base2.hpp"
#include "y/object.hpp"

namespace upsylon
{

    hash_slots_::~hash_slots_() throw()
    {
        object::dyadic_release(block_addr,block_exp2);
        block_addr = 0;
        _bzset(block_size);
        _bzset(block_mask);
        _bzset(block_exp2);
    }

    hash_slots_:: hash_slots_(const size_t n, const size_t slot_size) :
    block_size(0),
    block_mask(0),
    block_exp2(0),
    block_addr(0)
    {
        static const size_t min_size = 4;
        const        size_t max_size = base2<size_t>::max_power_of_two / slot_size;
        aliasing::_(block_size) = next_power_of_two( clamp(min_size,n,max_size) );
        aliasing::_(block_mask) = block_size-1;
        aliasing::_(block_exp2) = base2<size_t>::log2_of( next_power_of_two(block_size*slot_size) );
        block_addr = object::dyadic_acquire(block_exp2);
    }
}
