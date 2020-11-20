
//! \file

#ifndef Y_HASH_TABLE_INCLUDED
#define Y_HASH_TABLE_INCLUDED 1

#include "y/associative/hash/zpairs.hpp"
#include "y/associative/hash/buckets.hpp"


namespace upsylon
{
    //__________________________________________________________________________
    //
    //
    //! low-level hash-table of NODEs with KEY and Type
    /**
     NODE must have:
     - a 'hash_handle *meta' field
     - a const_key & key() const throw() function
     */
    //
    //__________________________________________________________________________
    template <typename NODE>
    class hash_table
    {
    public:
        typedef core::list_of<NODE> list_type; //!< alias

        //! setup with initial default buckets
        inline explicit hash_table() :
        nodes(),
        pails(0),
        cache()
        {
        }

        //! cleanup
        inline virtual ~hash_table() throw()
        {
        }
        

        //! no-throw swap
        inline void swap_with(hash_table &other) throw()
        {
            nodes.swap_with(other.nodes);
            pails.swap_with(other.pails);
            cache.swap_with(other.cache);
        }

        list_type         nodes;   //!< live nodes
        hash_buckets      pails;   //!< buckets of handles to nodes
        hash_zpairs<NODE> cache;   //!< cache
        
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(hash_table);
    };

}

#endif
