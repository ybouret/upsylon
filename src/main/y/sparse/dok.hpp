//! \file
#ifndef Y_SPARSE_DOK_INCLUDED
#define Y_SPARSE_DOK_INCLUDED 1

#include "y/associative/set.hpp"
#include "y/ptr/intr.hpp"

namespace upsylon
{

    namespace sparse
    {
        //______________________________________________________________________
        //
        //
        //! dictionary of key/item
        //
        //______________________________________________________________________
        template <typename KEY,
        typename T>
        class dok_item : public counted_object
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            Y_DECL_ARGS(T,type);                    //!< alias
            Y_DECL_ARGS(KEY,key_type);              //!< alias
            typedef intr_ptr<KEY,dok_item> pointer; //!< alias



            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //! initialize
            inline explicit dok_item(param_key_type k, param_type v) :
            __key(k), value(v) { }

            //! destructor
            inline virtual ~dok_item() throw() {}

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            //! key() for pointer/table
            inline const_key_type & key() const throw() { return __key; }

            //! display
            inline friend std::ostream & operator<<( std::ostream &os, const dok_item &item)
            {
                os << item.__key << ':' << item.value;
                return os;
            }

            //! content
            inline type       & operator*() throw()       { return value; }

            //! content, const
            inline const_type & operator*() const throw() { return value; }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const_key_type __key; //!< unique key
            type           value; //!< associated value

        private:
            Y_DISABLE_COPY_AND_ASSIGN(dok_item);
        };

        //______________________________________________________________________
        //
        //
        //! base class for dictionary of keys
        //
        //______________________________________________________________________
        class dok_base
        {
        public:
            virtual ~dok_base() throw(); //!< cleanup

        protected:
            explicit dok_base() throw();     //!< setup
            void     throw_bad_copy() const; //!< throw exception on bad copy

        private:
            Y_DISABLE_COPY_AND_ASSIGN(dok_base);
        };

        //______________________________________________________________________
        //
        //
        //! dictionary of keys = set of items
        //
        //______________________________________________________________________
        template <
        typename KEY,
        typename T,
        typename KEY_HASHER = key_hasher<KEY,hashing::fnv>,
        typename ALLOCATOR  = memory::global >
        class dok :
        public set<KEY, typename dok_item<KEY,T>::pointer, KEY_HASHER, ALLOCATOR>,
        public dok_base
        {
        public:
            Y_DECL_ARGS(T,type);                                           //!< alias
            Y_DECL_ARGS(KEY,key_type);                                     //!< alias
            typedef dok_item<KEY,T>                        item_type;      //!< alias
            typedef typename item_type::pointer            item_ptr;       //!< alias
            typedef set<KEY,item_ptr,KEY_HASHER,ALLOCATOR> self_type;      //!< alias
            typedef typename self_type::iterator           iterator;       //!< alias
            typedef typename self_type::const_iterator     const_iterator; //!< alias

            //! initalize with empty memory
            inline explicit dok() throw() : self_type(), dok_base()   {  }

            //! initialize with some memory
            inline explicit dok(const size_t n) : self_type(n,as_capacity), dok_base() {}

            //! cleanup
            inline virtual ~dok() throw() {}

            //! manually try to create a new entry
            inline bool create(param_key_type k, param_type v)
            {
                const item_ptr p = new item_type(k,v);
                return this->insert(p);
            }

            //! hard copy
            inline explicit dok(const dok &other) : collection(), self_type(other.size(),as_capacity), dok_base()
            {
                for( const_iterator i = other.begin(); i != other.end(); ++i )
                {
                    const item_ptr &p = *i;
                    const item_ptr  q = new item_type( p->__key, p->value );
                    if(!this->insert(q)) throw_bad_copy();
                }
            }

        private:
            Y_DISABLE_ASSIGN(dok);
        };
    }
}

#endif

