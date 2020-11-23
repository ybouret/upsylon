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
     - internal hash_table to handle NODEs
     - NODEs must have a 'data' field
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

        //! get load factor
        inline size_t load_factor() const throw()
        {
            return table.load_factor();
        }

        //! manual adjust (after removal...)
        inline void  try_update_load_factor() throw()
        {
            table.try_load_factor(ratio);
        }

        //! set number of buckets
        inline void set_buckets(const size_t n)
        {
            table.set_buckets(n);
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
        void sort_data_with(FUNC &func)
        {
            merging<NODE>::sort(table.nodes,compare_data<FUNC>,(void*)&func);
        }

        //! no-throw swap  
        inline void swap_with(hash_proto &other) throw()
        {
            table.swap_with(other.table);
            cswap(ratio,other.ratio);
        }

        //! slow access
        inline type & fetch(const size_t pos) throw()
        {
            return table.nodes.fetch(pos)->data;
        }

        //! slow const access
        inline const_type & fetch(const size_t pos) const throw()
        {
            return table.nodes.fetch(pos)->data;
        }

    protected:
        //! setup
        inline explicit hash_proto(const size_t factor) :
        base_type(),
        table(),
        ratio(factor),
        hash()
        {}
        
        //! setup
        inline explicit hash_proto(const hash_proto &other) :
        collection(),
        base_type(),
        table(other.table),
        ratio(other.ratio),
        hash()
        {}

        //! setup with capacity
        inline explicit hash_proto(const size_t         n,
                                   const as_capacity_t &_,
                                   const size_t         r) :
        base_type(),
        table(n,_,r),
        ratio(r),
        hash()
        {
        }

        //! post-insertion adjustment
        void post_insert()
        {
            if( table.load_factor()>ratio )
            {
                table.try_load_factor(ratio);
            }
        }

        
        table_type  table; //!< internal table

    private:
        Y_DISABLE_ASSIGN(hash_proto);

        template <typename FUNC>
        static inline int compare_data(const node_type *lhs, const node_type *rhs, void *args)
        {
            assert(args); assert(lhs); assert(rhs);
            FUNC &func = *(FUNC *)args;
            return func(lhs->data,rhs->data);
        }

        inline void zap( node_type *node ) throw()
        {
            assert(node);assert(node->meta);
            hash_meta *meta = node->meta;
            node->~NODE();
            table.pails[meta->hkey].unlink(meta);
            table.cache.store(meta);
        }

    public:
        size_t             ratio; //!< load factor for table
        mutable KEY_HASHER hash;  //!< key hasher

        typedef iterate::linked<type,node_type,iterate::forward>             iterator;        //!< forward iterator
        typedef iterate::linked<const_type,const node_type,iterate::forward> const_iterator;  //!< forward const iterator

        iterator begin() throw() { return iterator( table.nodes.head ); } //!< begin forward
        iterator end()   throw() { return iterator(0);                  } //!< end forward

        const_iterator begin() const throw()   { return const_iterator( table.nodes.head ); } //!< begin forward const
        const_iterator end()   const throw()   { return const_iterator(0);                  } //!< end forward const

        //! generic display
        inline friend std::ostream & operator<<( std::ostream &os, const hash_proto &proto )
        {
            os << '{';
            const node_type *node = proto.table.nodes.head;
            while(node)
            {
                os << ' ' << node->key() << ' ' << ':' << ' ' << node->data;
                const node_type *next = node->next;
                if(next)
                {
                    os << ',';
                }
                node=next;
            }
            os << '}';
            return os;
        }

        inline type       &front()       throw() { assert(table.nodes.head); return table.nodes.head->data; } //!< front
        inline const_type &front() const throw() { assert(table.nodes.head); return table.nodes.head->data; } //!< front
        inline type       &back()        throw() { assert(table.nodes.tail); return table.nodes.tail->data; } //!< back
        inline const_type &back()  const throw() { assert(table.nodes.tail); return table.nodes.tail->data; } //!< back

        inline void pop_back()  throw() { zap(table.nodes.pop_back());  } //!< remove back value
        inline void pop_front() throw() { zap(table.nodes.pop_front()); } //!< remove front value


        //! helper
        template <typename FUNC> inline
        void remove_key_if( FUNC &is_bad_key )
        {
            core::list_of<NODE> &l    = table.nodes;
            NODE                *node = l.head;
            while(node)
            {
                NODE *next = node->next;
                if( is_bad_key(node->key()) ) zap( l.unlink(node) );
                node=next;
            }
        }
    };

}

#endif
