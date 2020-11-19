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

        // release memory
        object::dyadic_release(slot,sexp2);

        // erase
        cleanup();
    }

    void hash_slots:: cleanup() throw()
    {
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
        // memory limits
        static const size_t min_size = 4;
        static const size_t max_size = base2<size_t>::max_power_of_two / sizeof(slot_type);

        // format
        aliasing::_(slots) = next_power_of_two( clamp(min_size,n,max_size) );
        aliasing::_(smask) = slots-1;
        aliasing::_(sexp2) = base2<size_t>::log2_of( next_power_of_two(slots*sizeof(slot_type)) );

        // acquire memory
        try
        {
            slot = static_cast<slot_type *>( object::dyadic_acquire(sexp2) );
        }
        catch(...)
        {
            cleanup();
            throw;
        }


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

    void hash_slots:: to(hash_slots &hs) throw()
    {
        for(size_t i=0;i<slots;++i)
        {
            slot_type &s = slot[i];
            while(s.size)
            {
                node_type *node = s.pop_back();
                hs[node->hkey].push_front(node);
            }
        }
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

#include <iostream>
#include <iomanip>

namespace upsylon
{
    void hash_slots:: dump() const
    {
        std::cerr << std::hex;
        std::cerr << "<hash_slots>" << std::endl;
        for(size_t i=0;i<slots;++i)
        {
            std::cerr << "[" << std::setw(4) << i << "] :";
            for(const node_type *node=slot[i].head;node;node=node->next)
            {
                std::cerr << " " << node->hkey;
            }
            std::cerr << std::endl;
        }

        std::cerr << "<hash_slots/>" << std::endl;
        std::cerr << std::dec;
    }
}

