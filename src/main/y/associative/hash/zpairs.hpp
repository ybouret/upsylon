
//! \file

#ifndef Y_HASH_ZPAIRS_INCLUDED
#define Y_HASH_ZPAIRS_INCLUDED 1

#include "y/associative/hash/bucket.hpp"
#include "y/associative/hash/znodes.hpp"


namespace upsylon
{

    //__________________________________________________________________________
    //
    //
    //! list of paired handles and zombie NODEs
    //
    //__________________________________________________________________________
    template <typename NODE>
    class hash_zpairs
    {
    public:
        typedef hash_bucket        h_list_type; //!< alias
        typedef hash_znodes<NODE>  z_list_type; //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! setup
        inline explicit hash_zpairs() throw() : hlist(), zlist()
        {
            assert( consistent() );
        }

        //! cleanup
        inline virtual ~hash_zpairs() throw()
        {
            assert(consistent());
            release();
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! check consistency
        inline bool consistent() const throw()
        {
            return hlist.size == zlist.size;
        }

        //! shared size
        inline size_t size() const throw()
        {
            assert( consistent() );
            return hlist.size;
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

        //! release all
        inline void release() throw()
        {
            while( size() ) pop();
        }

        //! return linked handle
        inline hash_handle *query(const size_t hkey) throw()
        {
            assert( consistent() );
            assert( hlist.size>0 );
            return hlist.query(hkey, zlist.query() );
        }

        //! store linked handle
        inline void store(hash_handle *handle) throw()
        {
            assert(handle);
            assert(handle->node);
            zlist.store( static_cast<NODE *>(handle->node) );
            hlist.store(handle);
        }

        //! no-throw swap
        inline void swap_with(hash_zpairs &other) throw()
        {
            hlist.swap_with(other.hlist);
            zlist.swap_with(other.zlist);
        }

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        h_list_type hlist; //!< list of handles
        z_list_type zlist; //!< list of zombie nodes

    private:
        Y_DISABLE_COPY_AND_ASSIGN(hash_zpairs);
    };

}

#endif
