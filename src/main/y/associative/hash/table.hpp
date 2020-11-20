
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

        //! search key and hkey within table
        template <typename KEY>
        NODE *search_node(const KEY     & key,
                          const size_t    hkey,
                          hash_bucket * & bucket) const throw()
        {
            //------------------------------------------------------------------
            // get bucket from hkey
            //------------------------------------------------------------------
            assert(0==bucket);
            bucket = (hash_bucket *)&pails[hkey];

            //------------------------------------------------------------------
            // loop over handle
            //------------------------------------------------------------------
            for(const hash_handle *meta=bucket->head;meta;meta=meta->next)
            {
                //--------------------------------------------------------------
                // test hkey first
                //--------------------------------------------------------------
                if(hkey!=meta->hkey) continue;

                //--------------------------------------------------------------
                // test full key
                //--------------------------------------------------------------
                const NODE *node = static_cast<const NODE *>(meta->node);
                assert(node);
                assert(meta==node->meta);
                if(key!=node->key()) continue;

                //--------------------------------------------------------------
                // found!
                //--------------------------------------------------------------
                return (NODE *)node;
            }

            //------------------------------------------------------------------
            // not found...
            //------------------------------------------------------------------
            return 0;
        }

        //! remove node with key and hkey
        template <typename KEY>
        bool remove_node(const KEY     & key,
                         const size_t    hkey) throw()
        {
            //------------------------------------------------------------------
            // search node with key and hkey
            //------------------------------------------------------------------
            hash_bucket *bucket = 0;
            NODE        *node   = search_node<KEY>(key,hkey,bucket);
            if(node)
            {
                //--------------------------------------------------------------
                // found the node
                //--------------------------------------------------------------
                assert(0!=bucket);
                assert(0!=node->meta);
                hash_handle *meta = node->meta;        // retrieve the handle
                nodes.unlink(node)->~NODE();           // destruct node content
                cache.store( bucket->unlink(meta) );   // back to cache
                return true;
            }
            else
            {
                //--------------------------------------------------------------
                // not found
                //--------------------------------------------------------------
                return false;
            }
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
