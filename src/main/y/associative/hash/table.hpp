
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
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef core::list_of<NODE> list_type; //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

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
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! no-throw swap
        inline void swap_with(hash_table &other) throw()
        {
            nodes.swap_with(other.nodes);
            pails.swap_with(other.pails);
            cache.swap_with(other.cache);
        }

        template <typename KEY>
        const NODE *search_node(typename type_traits<KEY>::parameter_type key,
                                const    size_t                           hkey,
                                hash_bucket                             *&bucket) const throw()
        {
            assert(0==bucket);
            bucket = (hash_bucket *)&pails[hkey];
            for(const hash_handle *meta=bucket->head;meta;meta=meta->next)
            {
                if(hkey!=meta->hkey) continue;
                const NODE *node = static_cast<const NODE *>(meta->node); assert(node);
                if(key!=node->key()) continue;
                return node;
            }
            return 0;
        }


        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        list_type         nodes;   //!< live nodes
        hash_buckets      pails;   //!< buckets of handles to nodes
        hash_zpairs<NODE> cache;   //!< cache
        
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(hash_table);
    };

}

#endif
