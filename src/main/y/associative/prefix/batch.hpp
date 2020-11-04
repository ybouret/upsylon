
//! \file

#ifndef Y_PREFIX_BATCH_INCLUDED
#define Y_PREFIX_BATCH_INCLUDED 1

#include "y/associative/prefix/tree.hpp"
#include "y/associative/catalog.hpp"
#include "y/type/class-conversion.hpp"

namespace upsylon {
    

    //! uses a suffix tree to support the catalog API
    template <typename KEY,typename T>
    class prefix_batch : public catalog<KEY,T>, public prefix_tree<char,T>
    {
    public:
        typedef catalog<KEY,T>      base_type; //!< alias
        typedef prefix_tree<char,T> tree_type; //!< alias
        Y_DECL_ARGS(T,type);                   //!< aliases
        Y_DECL_ARGS(KEY,key_type);             //!< aliases
                                            
        //! detect kind of key
        static const bool ro_buffer_key = Y_IS_SUPERSUBCLASS(memory::ro_buffer,mutable_key_type);
        
        //! setup
        inline explicit prefix_batch() :  base_type(), tree_type()
        {
        }
        
        //! setup with capacity
        inline explicit prefix_batch(const size_t n, const as_capacity_t &_) :
        base_type(), tree_type(n,_)
        {
        }
        
        //! hard copy
        inline prefix_batch( const prefix_batch &other ) :
        collection(),
        base_type(), tree_type(other)
        {
        }
        
        
        //! cleanup
        inline virtual ~prefix_batch() throw()
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
            return this->dl.size;
        }
        
        //! get capacity
        inline virtual size_t capacity() const throw()
        {
            return this->dl.size+this->dp.size;
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
        inline virtual bool insert(param_type args)
        {
            static const int2type<ro_buffer_key> kind = {};
            const_key_type & key = args.key();
            return this->__insert(__addr_of(key,kind),__size_of(key,kind),args);
        }
        
        
    private:
        Y_DISABLE_ASSIGN(prefix_batch);
        
        static inline const char * __addr_of( const_key_type &key, const int2type<false> & )  throw()
        {
            return (const char *)&key;
        }
        
        static inline size_t  __size_of( const_key_type &, const int2type<false> &)  throw()
        {
            return sizeof(KEY);
        }
        
        static inline const char * __addr_of( const_key_type &key, const int2type<true> & )  throw()
        {
            return static_cast<const char*>(key.ro());
        }
        
        static inline size_t  __size_of( const_key_type &key, const int2type<true> & )  throw()
        {
            return key.length();
        }
        
    };
    
}

#endif
