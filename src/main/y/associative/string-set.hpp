
//! \file
#ifndef Y_ASSOCIATIVE_STRING_SET_INCLUDED
#define Y_ASSOCIATIVE_STRING_SET_INCLUDED 1

#include "y/associative/set.hpp"
#include "y/string.hpp"
#include "y/ptr/intr.hpp"

namespace upsylon {

    //! pointer to string with self key()
    typedef intr_ptr<string,string> shared_string;

    //! a set of unique hashed strings
    template <
    typename KEY_HASHER = key_hasher<string>,
    typename ALLOCATOR  = memory::global>
    class string_set : public set<string,shared_string,KEY_HASHER,ALLOCATOR>
    {
    public:
        typedef set<string,shared_string,KEY_HASHER,ALLOCATOR> set_type; //!< alias

        //! setup
        inline explicit string_set() throw() : set_type() {}

        //! cleanup
        inline virtual ~string_set() throw() {}

        //! setup with memory
        inline explicit string_set(const size_t n, const as_capacity_t &_) : set_type(n,_) {}

        //! optimistic store
        inline bool store( const string &s )
        {
            const shared_string p = new string(s);
            return this->insert(p);
        }
        


    private:
        Y_DISABLE_COPY_AND_ASSIGN(string_set);
    };

}

#endif

