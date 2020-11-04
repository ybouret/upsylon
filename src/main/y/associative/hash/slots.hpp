//! \file
#ifndef Y_HASHED_SLOTS_INCLUDED
#define Y_HASHED_SLOTS_INCLUDED 1

#include "y/associative/hash/meta-pool.hpp"

namespace upsylon
{

    //__________________________________________________________________________
    //
    //
    //! slots for hash meta nodes
    //
    //__________________________________________________________________________
    class hash_slots
    {
    public:
        typedef hash_meta_node node_type;
        typedef hash_meta_list slot_type;

        virtual ~hash_slots() throw(); //!< cleanup and release memory
        explicit hash_slots(const size_t n);

        void     to(slot_type &repo) throw();

        void              load(hash_meta_node *node) throw();
        slot_type       & operator[](const size_t hkey) throw();
        const slot_type & operator[](const size_t hkey) const throw();

    private:
        slot_type   *slot;
    public:
        const size_t slots;        //!< number of slots, a power of two
        const size_t smask;        //!< mask for access = slots-1
        const size_t sexp2;        //!< ilog2(slots*sizeof(slot_type))
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(hash_slots);
    };




}


#endif
