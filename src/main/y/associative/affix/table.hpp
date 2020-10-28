//! \file

#ifndef Y_AFFIX_TABLE_INCLUDED
#define Y_AFFIX_TABLE_INCLUDED 1

#include "y/associative/affix/tree.hpp"
#include "y/associative/lexicon.hpp"
#include "y/type/class-conversion.hpp"

namespace upsylon {


    //! uses a suffix tree to support the lexicon API
    template <typename KEY,typename T>
    class affix_table : public lexicon<KEY,T>, public affix_tree<T>
    {
    public:
        typedef lexicon<KEY,T>     base_type; //!< alias
        typedef affix_tree<T>      tree_type; //!< alias
        Y_DECL_ARGS(T,type);                  //!< aliases
        Y_DECL_ARGS(KEY,key_type);            //!< aliases
        //! detect kind of key
        static const bool ro_buffer_key = Y_IS_SUPERSUBCLASS(memory::ro_buffer,mutable_key_type);

        //! setup
        inline explicit affix_table() :  base_type(), tree_type()
        {
        }

        //! setup with capacity
        inline explicit affix_table(const size_t n, const as_capacity_t &_) :
        base_type(), tree_type(n,_)
        {
        }

        //! hard copy
        inline affix_table( const affix_table &other ) :
        base_type(), tree_type(other)
        {
        }


        //! cleanup
        inline virtual ~affix_table() throw()
        {
        }

        //----------------------------------------------------------------------
        //
        // container interface
        //
        //----------------------------------------------------------------------

        //! get size
        inline virtual size_t size() const throw()
        {
            return this->dl_size();
        }

        //! get capacity
        inline virtual size_t capacity() const throw()
        {
            return this->dl_size() + this->dp_size();
        }

        //! free
        inline virtual void free() throw()
        {
            this->erase();
        }

        //! reserve
        inline virtual void reserve(const size_t n)
        {
            this->gain(n);
        }

        //! release all
        inline virtual void release() throw()
        {
            this->ditch();
        }

        //----------------------------------------------------------------------
        //
        // associative interface
        //
        //----------------------------------------------------------------------
        //! remove
        inline virtual bool remove(param_key_type key) throw()
        {
            static const int2type<ro_buffer_key> kind = {};
            return this->remove_by(__addr_of(key,kind),__size_of(key,kind));
        }

        //! search, const
        inline virtual const_type * search(param_key_type key) const throw()
        {
            static const int2type<ro_buffer_key> kind = {};
            return this->search_by(__addr_of(key,kind), __size_of(key,kind) );
        }

        //! search
        inline virtual type * search(param_key_type key) throw()
        {
            static const int2type<ro_buffer_key> kind = {};
            return (type *)(this->search_by(__addr_of(key,kind), __size_of(key,kind) ));
        }

        //----------------------------------------------------------------------
        //
        // type insertion
        //
        //----------------------------------------------------------------------
        //! insert using key as path
        inline virtual bool insert(param_key_type key, param_type args)
        {
            static const int2type<ro_buffer_key> kind = {};
            return this->insert_by(__addr_of(key,kind),__size_of(key,kind),args);
        }


    private:
        Y_DISABLE_ASSIGN(affix_table);

        static inline const void * __addr_of( const_key_type &key, const int2type<false> & )  throw()
        {
            return &key;
        }

        static inline size_t  __size_of( const_key_type &, const int2type<false> &)  throw()
        {
            return sizeof(KEY);
        }

        static inline const void * __addr_of( const_key_type &key, const int2type<true> & )  throw()
        {
            return key.ro();
        }

        static inline size_t  __size_of( const_key_type &key, const int2type<true> & )  throw()
        {
            return key.length();
        }

    };

}

#endif

