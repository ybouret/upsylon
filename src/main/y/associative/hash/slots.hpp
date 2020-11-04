//! \file
#ifndef Y_HASHED_SLOTS_INCLUDED
#define Y_HASHED_SLOTS_INCLUDED 1

#include "y/core/list.hpp"

namespace upsylon
{


    class hash_slots_
    {
    public:
        virtual ~hash_slots_() throw();

        const size_t block_size;
        const size_t block_mask;
        const size_t block_exp2;

    protected:
        void        *block_addr;
        explicit hash_slots_(const size_t n, const size_t slot_size);

    private:
        Y_DISABLE_COPY_AND_ASSIGN(hash_slots_);
    };


    template <typename NODE>
    class hash_slots : public hash_slots_
    {
    public:
        typedef core::list_of<NODE> slot_type;
        static const size_t         slot_size = sizeof(slot_type);

        explicit hash_slots(const size_t n) :
        hash_slots_(n,slot_size)
        {
            for(size_t i=0;i<block_size;++i)
            {
                new (slot+i) slot_type();
            }
        }

        inline virtual ~hash_slots() throw()
        {

        }

    private:
        slot_type    *slot;


        Y_DISABLE_COPY_AND_ASSIGN(hash_slots);
    };

}


#endif
