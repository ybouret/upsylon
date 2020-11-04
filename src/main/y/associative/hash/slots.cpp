#include "y/associative/hash/slots.hpp"
#include "y/type/block/zset.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/utils.hpp"
#include "y/code/base2.hpp"
#include "y/object.hpp"

namespace upsylon
{

    hash_slots::~hash_slots() throw()
    {
        // cleanup (will check empty slots)
        for(size_t i=0;i<slots;++i) slot[i].~slot_type();

        object::dyadic_release(slot,sexp2);
        slot = 0;
        _bzset(slots);
        _bzset(smask);
        _bzset(sexp2);
    }

    hash_slots:: hash_slots(const size_t n) :
    slot(0),
    slots(0),
    smask(0),
    sexp2(0)
    {
        static const size_t min_size = 4;
        const        size_t max_size = base2<size_t>::max_power_of_two / sizeof(slot_type);

        aliasing::_(slots) = next_power_of_two( clamp(min_size,n,max_size) );
        aliasing::_(smask) = slots-1;
        aliasing::_(sexp2) = base2<size_t>::log2_of( next_power_of_two(slots*sizeof(slot_type)) );
        slot               = static_cast<slot_type *>( object::dyadic_acquire(sexp2) );

        // finalize
        for(size_t i=0;i<slots;++i) new ( &slot[i] ) slot_type();

    }

    void hash_slots:: to(slot_type &repo) throw()
    {
        for(size_t i=0;i<slots;++i)
        {
            repo.merge_front(slot[i]);
            assert(0==slot[i].size);
        }
    }

    void hash_slots:: load(hash_meta_node *node) throw()
    {
        assert(node);
        slot[node->hkey&smask].push_front(node);
    }

    hash_slots::slot_type  &  hash_slots:: operator[](const size_t hkey) throw()
    {
        return slot[hkey&smask];
    }

    const hash_slots::slot_type  &  hash_slots:: operator[](const size_t hkey) const throw()
    {
        return slot[hkey&smask];
    }

}
