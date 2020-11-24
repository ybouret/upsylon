

//! \file

#ifndef Y_HASH_SLOTS_INCLUDED
#define Y_HASH_SLOTS_INCLUDED 1

#include "y/associative/hash/slot.hpp"
#include "y/code/base2.hpp"

namespace upsylon
{
    //__________________________________________________________________________
    //
    //
    //! array of slots
    //
    //__________________________________________________________________________
    class hash_slots
    {
    public:
        //______________________________________________________________________
        //
        // definitions
        //______________________________________________________________________
        static const size_t required_min_size = 4; //!< minimal slot count
        static const size_t max_size          = base2<size_t>::max_power_of_two / sizeof(hash_slot);         //!< maximal slots
        static const size_t min_size          = max_size < required_min_size ? max_size : required_min_size; //!< minimal slots

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        hash_slots(const size_t n); //!< setup next_power_of_two(clamp(min_size,n,max_size)) buckets
        ~hash_slots() throw();      //!< release all memory

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        void swap_with(hash_slots &other) throw();                       //!< no-throw swap
        void release() throw();                                          //!< release all slots

        hash_slot       & operator[](const size_t hkey)       throw(); //!< access
        const hash_slot & operator[](const size_t hkey) const throw(); //!< const access

        void       to(hash_slot  &pool)     throw(); //!< move all content to pool
        void       to(hash_slots &other)    throw(); //!< redispatch nodes
        void       insert(hash_meta *meta)  throw(); //!< insert meta w.r.t its key
        hash_meta *remove(hash_meta *meta)  throw(); //!< remove from its bucket
        void       dump() const;                     //!< display each slot: keys

        //! count of slots to ensure entries/count <= load_factor
        static size_t required_for(const size_t load_factor, const size_t entries) throw();
        
        bool try_resize_for(const size_t load_factor,
                            const size_t entries) throw();

        bool try_resize(const size_t required) throw();


    private:
        hash_slot   *slot;
    public:
        const size_t count;      //!< number of buckets, a power of two
        const size_t bmask;      //!< mask for access = slots-1
        const size_t bexp2;      //!< ilog2(slots*sizeof(hash_bucket))

    private:
        Y_DISABLE_COPY_AND_ASSIGN(hash_slots);
        hash_slots(const size_t, const size_t, const size_t);
        void cleanup() throw();
        void setup();
    };


}

#endif
