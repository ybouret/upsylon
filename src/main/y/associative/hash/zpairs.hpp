
//! \file

#ifndef Y_HASH_ZPAIRS_INCLUDED
#define Y_HASH_ZPAIRS_INCLUDED 1

#include "y/associative/hash/bucket.hpp"
#include "y/associative/hash/znodes.hpp"


namespace upsylon
{

    template <typename NODE>
    class hash_zpairs
    {
    public:
        typedef hash_bucket        h_list_type;
        typedef hash_znodes<NODE>  z_list_type;

        inline explicit hash_zpairs() throw() : hlist(), zlist()
        {
            assert( consistent() );
        }


        inline bool consistent() const throw()
        {
            return hlist.size == zlist.size;
        }

        //! push one of each
        inline void push()
        {
            assert( consistent() );
            {      hlist.push(); }
            try {  zlist.push(); } catch(...) { hlist.pop(); assert( consistent() ); throw; }
            assert( consistent() );
        }

        //! pop one of each
        inline void pop() throw()
        {
            assert( consistent() );
            hlist.pop();
            zlist.pop();
            assert( consistent() );
        }

        h_list_type hlist;
        z_list_type zlist;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(hash_zpairs);
    };

}

#endif
