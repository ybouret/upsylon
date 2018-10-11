
//! \file
#ifndef Y_HASH_CONTAINER_INCLUDED
#define Y_HASH_CONTAINER_INCLUDED 1

#include "y/associative/hash-table.hpp"
#include "y/container/associative.hpp"
#include "y/hashing/key-hasher.hpp"
#include "y/iterate/meta.hpp"

namespace upsylon
{
    //! base functions for set/map
    template <
    typename KEY,
    typename T,
    typename NODE,
    typename KEY_HASHER = key_hasher<KEY>,
    typename ALLOCATOR  = memory::global >
    class hash_container : public associative<KEY,T>
    {
    public:
        Y_DECL_ARGS(T,type);       //!< alias
        Y_DECL_ARGS(KEY,key_type); //!< alias
        typedef core::hash_table<NODE,ALLOCATOR> table_type; //!< internal table
        typedef typename table_type::slot_type   slot_type;  //!< slot of data
        typedef typename table_type::meta_node   meta_node;  //!< meta node
        typedef associative<KEY,T>               base_type;  //!< base type 
        //! destructor
        inline virtual ~hash_container() throw() {}

        //! default constructor
        inline explicit hash_container() throw() : dynamic(), base_type(), table(), hash() {}

        //! constructor with capacity
        inline explicit hash_container(const size_t n, const as_capacity_t &) : dynamic(), base_type(), table(n), hash() {}

        //! container interface: size
        inline virtual size_t size() const throw() { return table.chain.size; }

        //! container interface: capacity
        inline virtual size_t capacity() const throw() { return table.items; }

        //! container interface: free
        inline virtual void free() throw() { table.free(); }

        //! container interface: release
        inline virtual void release() throw() { table_type nil; table.swap_with(nil); }

        //! container interface: reserve
        inline virtual void reserve( const size_t n )
        {
            table_type tmp( this->size() + n );
            tmp.duplicate(table);
            tmp.swap_with(table);
        }

        //! copy
        inline hash_container(const hash_container &other) : dynamic(), base_type(), table(other.table), hash() {}

        //! assign by duplication
        inline hash_container & operator=( const hash_container &other )
        {
            table_type tmp( other.table );
            table.swap_with(tmp);
            return *this;
        }

        //! search
        inline virtual type *search( param_key_type k ) throw()
        {
            slot_type   *s = 0;
            const size_t h = hash(k);
            const NODE  *n = table.template search_node<KEY>(k,h, &s);
            if(n)
            {
                NODE *node = (NODE *)n;
                // TODO: move to front ?
                return &(node->data);
            }
            else
            {
                return 0;
            }
        }
        
        //! search, const type
        inline virtual const_type *search( param_key_type k ) const throw()
        {
            slot_type   *s = 0;
            const size_t h = hash(k);
            const NODE  *n = table.template search_node<KEY>(k,h, &s);
            if(n)
            {
                return &(n->data);
            }
            else
            {
                return 0;
            }
        }
        
        //! remove object with given key, if found
        inline virtual bool remove( param_key_type k ) throw()
        {
            slot_type   *s = 0;
            const size_t h = hash(k);
            const NODE  *n = table.template search_node<KEY>(k,h, &s);
            if(n)
            {
                assert(s); assert(s->owns(n));
                table.__free( s->unlink((NODE *)n) );
                return true;
            }
            else
            {
                return false;
            }
        }

        //! sort internal nodes by data
        template <typename COMPARE_DATA>
        inline void sort_data( COMPARE_DATA &compare_data ) { table.sort_by_data(compare_data); }

        //! sort internal nodes by keys
        template <typename COMPARE_KEYS>
        inline void sort_keys( COMPARE_KEYS &compare_keys ) { table.sort_by_keys(compare_keys); }

        //! reverse order of internal nodes
        inline void reverse() throw() { table.chain.reverse(); }

    protected:
        table_type table; //!< handle low level operations
        
    public:
        mutable KEY_HASHER hash; //!< hasher for the key type

        typedef iterate::meta<key_type,type,meta_node,iterate::forward> iterator; //!< forward iterator
        inline  iterator begin() throw() { return iterator(table.chain.head); }   //!< first forward iterator
        inline  iterator end()   throw() { return iterator(NULL); }               //!< last  forwatd iterator

        typedef iterate::meta<key_type,const_type,const meta_node,iterate::forward> const_iterator; //!< const iterator
        inline  const_iterator begin() const throw() { return const_iterator(table.chain.head); }   //!< first const forward iterator
        inline  const_iterator end()   const throw() { return const_iterator(NULL); }               //!< last  const forward iterator



    };
}
#endif
