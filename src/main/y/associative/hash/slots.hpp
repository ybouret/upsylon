//! \file
#ifndef Y_HASHED_SLOTS_INCLUDED
#define Y_HASHED_SLOTS_INCLUDED 1

#include "y/core/list.hpp"

namespace upsylon
{

    //__________________________________________________________________________
    //
    //
    //! base class for hash_slots: metric and memory
    //
    //__________________________________________________________________________
    class hash_slots_
    {
    public:
        virtual ~hash_slots_() throw(); //!< cleanup and release memory

        const size_t slots;             //!< number of slots, a power of two
        const size_t smask;             //!< mask for access = slots-1
        const size_t block_exp2;        //!< ilog2(slots*slot_size)

    protected:
        void        *block_addr;   //!< address of first slot

        //! compute metrics and acquire memory
        explicit hash_slots_(const size_t n,
                             const size_t slot_size);

    private:
        Y_DISABLE_COPY_AND_ASSIGN(hash_slots_);
    };


    //__________________________________________________________________________
    //
    //
    //! hash slots for a given NODE
    //
    //__________________________________________________________________________
    template <typename NODE>
    class hash_slots : public hash_slots_
    {
    public:
        //______________________________________________________________________
        //
        // types and definition
        //______________________________________________________________________
        typedef core::list_of<NODE> slot_type;                     //!< alias
        static const size_t         slot_size = sizeof(slot_type); //!< alias


        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! setup
        explicit hash_slots(const size_t n) :
        hash_slots_(n,slot_size),
        slot( static_cast<slot_type *>(block_addr) )
        {
            for(size_t i=0;i<slots;++i)
            {
                new (slot+i) slot_type();
            }
        }

        //! cleanup
        inline virtual ~hash_slots() throw()
        {
            for(size_t i=0;i<slots;++i)
            {
                slot[i].~slot_type();
            }
            slot = 0;
        }

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        
        slot_type    *slot; //!< slots[0..slots-1]

    private:
        Y_DISABLE_COPY_AND_ASSIGN(hash_slots);
    };

}


#endif
