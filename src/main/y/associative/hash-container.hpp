
//! \file
#ifndef Y_HASH_CONTAINER_INCLUDED
#define Y_HASH_CONTAINER_INCLUDED 1

#include "y/associative/hash-table.hpp"
#include "y/container/associative.hpp"
#include "y/hashing/key-hasher.hpp"
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
        
        //! destructor
        inline virtual ~hash_container() throw() {}

        //! default constructor
        inline explicit hash_container() throw() : table(), hash() {}

        //! constructor with capacity
        inline explicit hash_container(const size_t n, const as_capacity_t &) : table(n), hash() {}

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
        inline hash_container(const hash_container &other) : table(other.table) {}

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
        
    protected:
        table_type table; //!< handle low level operations

    public:
        mutable KEY_HASHER hash; //!< hasher for the key type

    };
}
#endif
