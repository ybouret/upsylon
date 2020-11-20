
//! \file

#ifndef Y_HASH_TABLE_INCLUDED
#define Y_HASH_TABLE_INCLUDED 1

#include "y/associative/hash/zpairs.hpp"
#include "y/associative/hash/buckets.hpp"


namespace upsylon
{
    //__________________________________________________________________________
    //
    //! base class for parameters
    //__________________________________________________________________________
     class hash_table_
    {
    public:
        static const size_t default_load = 8; //!< default load factor
        virtual ~hash_table_() throw();       //!< cleanup
        
    protected:
        explicit hash_table_() throw();       //!< setup
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(hash_table_);
    };
    
    //__________________________________________________________________________
    //
    //
    //! low-level hash-table of NODEs with KEY and Type
    /**
     NODE must have:
     - a 'hash_handle *meta' field
     - a const_key & key() const throw() function
     - a constructor NODE(key,data)
     */
    //
    //__________________________________________________________________________
    template <typename NODE>
    class hash_table : public hash_table_
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
        hash_table_(),
        nodes(),
        pails(0),
        cache()
        {
        }
        
        //! setup with memory
        inline explicit hash_table(const size_t         n,
                                   const as_capacity_t &,
                                   const size_t        load = default_load) :
        hash_table_(),
        nodes(),
        pails( hash_buckets::for_load_factor(load,n) ),
        cache()
        {
            reserve(n);
        }

        //! copy
        inline explicit hash_table( const hash_table &other ) :
        hash_table_(),
        nodes(),
        pails(other.pails.count),
        cache()
        {
            try
            {
                for(const NODE *source=other.nodes.head;source;source=source->next)
                {
                    // setup memory
                    cache.push();
                    const size_t hkey = source->meta->hkey;
                    hash_meta   *meta = cache.query(hkey);
                    NODE        *node = static_cast<NODE *>(meta->node);
                    
                    // node copy constructor
                    try { new (node) NODE( *source ); }
                    catch(...) { cache.store(meta); throw; }
                    
                    // setup node
                    setup(node,meta);
                }
            }
            catch(...)
            {
                release();
                throw;
            }
        }
        

        //! cleanup
        inline virtual ~hash_table() throw()
        {
            release();
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

        //! populate cache
        inline void reserve(size_t n)
        {
            while(n-- > 0) cache.push();
        }
        
        //! capacity
        inline size_t capacity() const throw()
        {
            return nodes.size + cache.size();
        }
        
        //! trim = release cache
        inline void trim() throw()
        {
            cache.release();
        }

        //! average load factor
        inline size_t load_factor() const throw()
        {
            return nodes.size / pails.count;
        }

        //! number of buckets to reach requested load factor
        inline size_t buckets_for_load_factor(const size_t value) const throw()
        {
            return hash_buckets::for_load_factor(value,nodes.size);
        }

        //! setup new load factor
        inline void load_factor(const size_t value)
        {
            const size_t required_buckets = buckets_for_load_factor(value);
            if(required_buckets!=pails.count)
            {
                hash_buckets temp(required_buckets);
                pails.to(temp);
                pails.swap_with(temp);
            }
        }

        //! try to setup a new load factor, no-throw
        inline void try_load_factor(const size_t value) throw()
        {
            try { load_factor(value); } catch(...) { }
        }


        //! free node content, keep memory
        inline void free() throw()
        {
            while(nodes.size)
            {
                NODE      *node = nodes.pop_back();
                hash_meta *meta = node->meta;
                node->~NODE();
                cache.store( pails[meta->hkey].unlink(meta) );
            }
        }



        //! release all possible memory
        inline void release() throw()
        {
            trim();
            while(nodes.size)
            {
                NODE      *node = nodes.pop_back();
                hash_meta *meta = node->meta;
                node->~NODE();
                object::release1(node);
                hash_meta::release( pails[meta->hkey].unlink(meta) );
            }
        }


        //! search key and hkey within table
        template <typename KEY> inline
        NODE *search(const KEY     & key,
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
            for(const hash_meta *meta=bucket->head;meta;meta=meta->next)
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
        template <typename KEY> inline
        bool remove(const KEY     & key,
                    const size_t    hkey) throw()
        {
            //------------------------------------------------------------------
            // search node with key and hkey
            //------------------------------------------------------------------
            hash_bucket *bucket = 0;
            NODE        *node   = search<KEY>(key,hkey,bucket);
            if(node)
            {
                //--------------------------------------------------------------
                // found the node
                //--------------------------------------------------------------
                assert(0!=bucket);
                assert(0!=node->meta);
                hash_meta *meta = node->meta;          // retrieve the handle
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

        //! insert node with key, hkey and data
        template <typename KEY, typename T> inline
        bool insert(const KEY  & key,
                    const size_t hkey,
                    const T    & data)
        {
            hash_bucket *bucket = 0;
            if(search<KEY>(key,hkey,bucket))
            {
                //--------------------------------------------------------------
                // already there
                //--------------------------------------------------------------
                return false;
            }
            else
            {
                //--------------------------------------------------------------
                // get raw memory
                //--------------------------------------------------------------
                if(cache.size()<=0) cache.push();
                hash_meta   *meta = cache.query(hkey);
                NODE        *node = static_cast<NODE *>(meta->node);

                //--------------------------------------------------------------
                // construct node
                //--------------------------------------------------------------
                try
                {
                    new (node) NODE(key,data);
                }
                catch(...)
                {
                    cache.store(meta);
                    throw;
                }

                //--------------------------------------------------------------
                // update structure
                //--------------------------------------------------------------
                setup(node,meta);
                return true;
            }
            
        }


        //! sort nodes by keys
        template <typename FUNC> inline
        void sort_keys(FUNC &compare)
        {
            merging<NODE>::sort(nodes,compare_keys<FUNC>,(void *)&compare);
        }
        


        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        list_type         nodes;   //!< live nodes
        hash_buckets      pails;   //!< buckets of handles to nodes
        hash_zpairs<NODE> cache;   //!< cache
        
        
    private:
        Y_DISABLE_ASSIGN(hash_table);
        template <typename FUNC> static inline
        int compare_keys(const NODE *lhs, const NODE *rhs, void *args)
        {
            assert(args);
            FUNC &compare = *(FUNC *)args;
            return compare(lhs->key(),rhs->key());
        }
        
        inline void setup(NODE *node, hash_meta *meta) throw()
        {
            pails.insert( nodes.push_back(node)->meta = meta );
        }
    };

}

#endif
