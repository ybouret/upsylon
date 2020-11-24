

#include "y/associative/hash/slots.hpp"
#include "y/type/utils.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/self-destruct.hpp"
#include "y/object.hpp"

namespace upsylon
{

    hash_slots:: ~hash_slots() throw()
    {
        for(size_t i=0;i<count;++i)
        {
            self_destruct(slot[i]);
        }
        object::dyadic_release(slot,bexp2);
        cleanup();
    }

    void hash_slots:: cleanup() throw()
    {
        slot = 0;
        aliasing::_(count) = 0;
        aliasing::_(bmask) = 0;
        aliasing::_(bexp2) = 0;
    }

    void hash_slots:: release() throw()
    {
        for(size_t i=0;i<count;++i)
        {
            slot[i].release();
        }
    }

    void hash_slots:: swap_with(hash_slots &other) throw()
    {
        _cswap(slot,other.slot);
        _cswap(count,other.count);
        _cswap(bmask,other.bmask);
        _cswap(bexp2,other.bexp2);
    }

    static inline size_t bmask_for(const size_t count) throw()
    {
        assert( is_a_power_of_two(count) );
        return count-1;
    }

    static inline size_t slots_for(const size_t n) throw()
    {
        return next_power_of_two<size_t>( clamp(hash_slots::min_size,n,hash_slots::max_size) );
    }


    static inline size_t bexp2_for(const size_t count) throw()
    {
        return base2<size_t>::log2_of(count*sizeof(hash_slot) );
    }

    void hash_slots:: setup()
    {
        // acquire memory
        try
        {
            slot = static_cast<hash_slot *>( object::dyadic_acquire(bexp2) );
        }
        catch(...)
        {
            cleanup();
            throw;
        }

        // finalize
        for(size_t i=0;i<count;++i) new (slot+i) hash_slot();
    }

    hash_slots:: hash_slots(const size_t n) :
    slot(0),
    count( slots_for(n)     ),
    bmask( bmask_for(count) ),
    bexp2( bexp2_for(count) )
    {
        setup();
    }

    hash_slots:: hash_slots(const size_t __count,
                            const size_t __bmask,
                            const size_t __bexp2) :
    slot(0),
    count(__count),
    bmask(__bmask),
    bexp2(__bexp2)
    {
        assert(count>=min_size);
        assert(count<=max_size);
        assert(count-1==bmask);
        setup();
    }


    hash_slot & hash_slots:: operator[](const size_t hkey) throw()
    {
        return slot[hkey&bmask];
    }

    const hash_slot & hash_slots:: operator[](const size_t hkey) const throw()
    {
        return slot[hkey&bmask];
    }


    void hash_slots:: to(hash_slot &pool) throw()
    {
        for(size_t i=0;i<count;++i)
        {
            pool.merge_back(slot[i]);
        }
    }

    void hash_slots:: to(hash_slots &other) throw()
    {
        for(size_t i=0;i<count;++i)
        {
            hash_slot &b = slot[i];
            while(b.size)
            {
                other.insert( b.pop_back() );
            }
        }
    }

    void  hash_slots:: insert(hash_meta *handle) throw()
    {
        assert(handle);
        (*this)[handle->hkey].push_front(handle);
    }

    hash_meta * hash_slots:: remove(hash_meta *meta) throw()
    {
        assert(meta);
        return (*this)[meta->hkey].unlink(meta);
    }

    static inline
    size_t raw_required_for(const size_t load_factor, const size_t entries) throw()
    {
        if(load_factor<=1)
        {
            return entries;
        }
        else
        {
            size_t n = entries/load_factor;
            while(n*load_factor<entries) ++n;
            return n;
        }
    }

    size_t hash_slots:: required_for(const size_t load_factor, const size_t entries) throw()
    {
        return slots_for( raw_required_for(load_factor,entries) );
    }

    bool hash_slots:: try_resize_for(const size_t load_factor,
                                     const size_t entries) throw()
    {
        return try_resize( raw_required_for(load_factor,entries) );
    }


    bool hash_slots:: try_resize(const size_t required) throw()
    {
        try
        {
            // initialize
            const size_t new_count =  slots_for(required);
            if(new_count!=count)
            {
                // memory acquisition
                hash_slots new_slots(new_count,bmask_for(new_count),bexp2_for(new_count));

                // exchange
                to(new_slots);
                swap_with(new_slots);
            }
            return true;
        }
        catch(...)
        {
            return false;
        }
    }

}


#include <iostream>
#include <iomanip>

namespace upsylon
{


    void hash_slots:: dump() const
    {
        std::cerr << std::hex;
        std::cerr << "<hash_buckets>" << std::endl;
        for(size_t i=0;i<count;++i)
        {
            std::cerr << "[" << std::setw(4) << i << "] :";
            for(const hash_meta *node=slot[i].head;node;node=node->next)
            {
                std::cerr << " " << node->hkey;
            }
            std::cerr << std::endl;
        }

        std::cerr << "<hash_buckets/>" << std::endl;
        std::cerr << std::dec;
    }
}
