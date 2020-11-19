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
        typedef hash_meta_node node_type; //!< alias
        typedef hash_meta_list slot_type; //!< alias

        virtual ~hash_slots() throw();       //!< cleanup and release memory
        explicit hash_slots(const size_t n); //!< setup for required n slots

        void     to(slot_type  &repo) throw(); //!< all nodes to repo
        void     to(hash_slots &hs)   throw(); //!< all nodes to other

        slot_type       & operator[](const size_t hkey) throw();       //!< access
        const slot_type & operator[](const size_t hkey) const throw(); //!< access

        void dump() const; //!< output

    private:
        slot_type   *slot;
    public:
        const size_t slots;        //!< number of slots, a power of two
        const size_t smask;        //!< mask for access = slots-1
        const size_t sexp2;        //!< ilog2(slots*sizeof(slot_type))
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(hash_slots);
        void cleanup() throw();
    };




}


#endif
