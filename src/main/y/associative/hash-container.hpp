
//! \file
#ifndef Y_HASH_CONTAINER_INCLUDED
#define Y_HASH_CONTAINER_INCLUDED 1

#include "y/associative/hash-table.hpp"
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
    class hash_container : public container
    {
    public:
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

    protected:
        typedef core::hash_table<NODE,ALLOCATOR> table_type;
        table_type table;

    public:
        KEY_HASHER hash;

    };
}
#endif
