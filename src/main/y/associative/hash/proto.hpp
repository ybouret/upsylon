//! \file

#ifndef Y_HASH_PROTO_INCLUDED
#define Y_HASH_PROTO_INCLUDED 1

#include "y/associative/hash/table.hpp"
#include "y/hashing/key-hasher.hpp"
#include "y/iterate/linked.hpp"
#include "y/collection.hpp"

namespace upsylon
{
    //__________________________________________________________________________
    //
    //! prototype of hash_table based container
    /**
     - BASE_TYPE: derived with virtual collection
     */
    //__________________________________________________________________________
    template <
    typename KEY,
    typename T,
    typename NODE,
    typename KEY_HASHER,
    typename BASE_TYPE>
    class hash_proto : public BASE_TYPE
    {
    public:
        //______________________________________________________________________
        //
        // types and declaration
        //______________________________________________________________________
        Y_DECL_ARGS(T,type);                  //!< aliases
        Y_DECL_ARGS(KEY,key_type);            //!< aliases
        typedef NODE             node_type;   //!< alias
        typedef BASE_TYPE        base_type;   //!< alias
        typedef hash_table<NODE> table_type;  //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline virtual ~hash_proto() throw() {} //!< cleanup


        //______________________________________________________________________
        //
        // container interface
        //______________________________________________________________________
        inline virtual size_t size()     const throw() { return table.nodes.size; } //!< container: size
        inline virtual size_t capacity() const throw() { return table.capacity(); } //!< container: capacity
        inline virtual void   reserve(const size_t n)  { table.reserve(n);        } //!< container: reserve
        inline virtual void   free() throw()           { table.free();            } //!< container: free
        inline virtual void   release() throw()        { table.release();         } //!< container: release
        
        //______________________________________________________________________
        //
        // associative interface
        //______________________________________________________________________
        //! associative: search in constant table
        inline virtual const_type *search(param_key_type key) const throw()
        {
            hash_bucket     *slot = 0;
            const size_t     hkey = hash(key);
            const node_type *node = table. template search<KEY>(key,hkey,slot);
            if(node)
            {
                return &(node->data);
            }
            else
            {
                return 0;
            }
        }
        
        //! associative: search in table
        inline virtual  type *search(param_key_type key) throw()
        {
            hash_bucket  *slot = 0;
            const size_t  hkey = hash(key);
            node_type    *node = table. template search<KEY>(key,hkey,slot);
            if(node)
            {
                return &(node->data);
            }
            else
            {
                return 0;
            }
        }
        
        //! removal
        inline virtual bool remove(param_key_type key) throw()
        {
            const size_t  hkey = hash(key);
            return table. template remove<KEY>(key,hkey);
        }

        //! collect keys
        template <typename SEQUENCE> inline
        void collect_keys(SEQUENCE &keys) const
        {
            for(const node_type *node=table.nodes.head;node;node=node->next)
            {
                keys << node->key();
            }
        }

        //! wrapper to sort by keys
        template <typename FUNC> inline
        void sort_keys_with(FUNC &func)
        {
            table. template sort_keys<FUNC>(func);
        }

        //! wrapper to sort data with func
        template <typename FUNC> inline
        void sort_with(FUNC &func)
        {
            table. template sort_with<FUNC>(func);
        }


    protected:
        //! setup
        inline explicit hash_proto() :
        base_type(),
        table(),
        hash()
        {}
        
        //! setup
        inline explicit hash_proto(const hash_proto &other) :
        collection(),
        base_type(),
        table(other.table),
        hash()
        {}

        //! setup with capacity
        inline explicit hash_proto(const size_t n, const as_capacity_t &_) :
        base_type(),
        table(n,_),
        hash()
        {
        }

        //! post-insertion adjustment
        void post_insert()
        {
        }

        //! no-throw swap for derived classes
        inline void swap_with(hash_proto &other) throw()
        {
            table.swap_with(other.table);
        }

        table_type  table; //!< internal table

    private:
        Y_DISABLE_ASSIGN(hash_proto);

    public:
        mutable KEY_HASHER hash; //!< key hasher

        typedef iterate::linked<type,node_type,iterate::forward>             iterator;        //!< forward iterator
        typedef iterate::linked<const_type,const node_type,iterate::forward> const_iterator;  //!< forward const iterator

        iterator begin() throw() { return iterator( table.nodes.head ); } //!< begin forward
        iterator end()   throw() { return iterator(0);                  } //!< end forward

        const_iterator begin() const throw()   { return const_iterator( table.nodes.head ); } //!< begin forward const
        const_iterator end()   const throw()   { return const_iterator(0);                  } //!< end forward const

        
    };

}

#endif
