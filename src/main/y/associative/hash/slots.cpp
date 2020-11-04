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
        _bzset(slots);
        _bzset(smask);
        _bzset(block_exp2);
    }

    hash_slots_:: hash_slots_(const size_t n, const size_t slot_size) :
    slots(0),
    smask(0),
    block_exp2(0),
    block_addr(0)
    {
        static const size_t min_size = 4;
        const        size_t max_size = base2<size_t>::max_power_of_two / slot_size;
        aliasing::_(slots)      = next_power_of_two( clamp(min_size,n,max_size) );
        aliasing::_(smask)      = slots-1;
        aliasing::_(block_exp2) = base2<size_t>::log2_of( next_power_of_two(slots*slot_size) );
        block_addr = object::dyadic_acquire(block_exp2);
    }
}
