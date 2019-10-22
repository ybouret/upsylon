//! \file

#ifndef Y_BTABLE_INCLUDED
#define Y_BTABLE_INCLUDED 1

#include "y/associative/b-tree.hpp"
#include "y/type/class-conversion.hpp"
#include "y/iterate/linked.hpp"
//#include <iostream>

namespace upsylon
{

    namespace core
    {
        //! light-weight key
        struct lw_key
        {
            const void *addr; //!< any address
            size_t      size; //!< any size
        };

        //! make default light-weight keys
        template <typename T>
        class lw_key_maker
        {
        public:
            Y_DECL_ARGS(T,type);              //!< aliaa
            inline  lw_key_maker() throw() {} //!< setup
            inline ~lw_key_maker() throw() {} //!< cleanup

            //! build keys, assuming buffer or integral type
            inline void operator()(lw_key &lwk, const_type & key ) const throw()
            {
                 return fill(lwk, key, int2type< Y_IS_SUPERSUBCLASS(memory::ro_buffer,mutable_type) >() );
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(lw_key_maker);
            void fill(lw_key &lwk, const_type &key, int2type<true> ) const throw()
            {
                lwk.addr = key.ro();
                lwk.size = key.length();
            }

            void fill(lw_key &lwk, const_type &key, int2type<false> ) const throw()
            {
                lwk.addr = &key;
                lwk.size = sizeof(type);
            }


        };
    }


    //! B-Table implementation for a named KEY
    template <typename KEY, typename T, typename KEY_MAKER = core::lw_key_maker<KEY> >
    class btable : public btree<T>, public associative<KEY,T>
    {
    public:
        Y_DECL_ARGS(KEY,key_type);                       //!< alias
        Y_DECL_ARGS(T,type);                             //!< alias
        typedef associative<KEY,T>           base_type;  //!< alias
        typedef typename btree<T>::data_node dnode_type; //!< alias

        //! setup
        inline explicit btable() throw() : btree<T>(), base_type(), key_maker()
        {
        }

        //! setup with some capacity
        inline explicit btable( const size_t n, const as_capacity_t &_ ) : btree<T>(n,_), base_type(), key_maker()
        {
        }

        //! cleanup
        inline virtual ~btable() throw()
        {
        }

        //! copy
        inline btable( const btable &other ) : collection(), btree<T>(other), base_type(), key_maker() {}

        //! assign by copy/swap
        inline btable & operator=( const btable &other )
        {
            btable temp(other);
            this->swap_with(temp);
            return *this;
        }

        inline virtual size_t size() const throw()     { return this->dlist.size; }                  //! container interface : size()
        inline virtual void   free()       throw()     { this->free_();      }                       //! container interface : free()
        inline virtual void   release()    throw()     { this->release_();   }                       //! container interface : release()
        inline virtual size_t capacity() const throw() { return this->dlist.size+this->dpool.size; } //! container interface : capacity()
        inline virtual void   reserve(const size_t n)  { this->reserve_(n);  }                       //! container interface : reserve()

        //! associative interface: remove
        inline virtual bool remove( param_key_type k ) throw()
        {
            core::lw_key lwk = {0,0};
            key_maker(lwk,k);
            return this->remove_(lwk.addr,lwk.size);
        }

        //! associative interface: search
        virtual type *search( param_key_type k ) throw()
        {
            return (type *)find__(k);
        }

        //! associative interface: search, const
        virtual const_type *search( param_key_type k ) const throw()
        {
            return  find__(k);
        }

        //! specific interface
        inline bool insert( param_key_type k, param_type v )
        {
            core::lw_key lwk = {0,0};
            key_maker(lwk,k);
            return this->insert_(lwk.addr,lwk.size,v);
        }
        
        typedef iterate::linked<type,dnode_type,iterate::forward>             iterator;        //!< forward iterator
        typedef iterate::linked<const_type,const dnode_type,iterate::forward> const_iterator;  //!< forward const iterator

        iterator begin() throw() { return iterator( this->dlist.head ); } //!< begin forward
        iterator end()   throw() { return iterator(0);                  } //!< end forward

        const_iterator begin() const throw()   { return const_iterator(this->dlist.head ); } //!< begin forward const
        const_iterator end()   const throw()   { return const_iterator(0);                 } //!< end forward const

        typedef iterate::linked<type,dnode_type,iterate::reverse>             reverse_iterator;        //!< reverse iterator
        typedef iterate::linked<const_type,const dnode_type,iterate::reverse> const_reverse_iterator;  //!< reverse const iterator

        reverse_iterator rbegin() throw() { return reverse_iterator( this->dlist.tail ); } //!< begin reverse
        reverse_iterator rend()   throw() { return reverse_iterator(0);                  } //!< end reverse

        const_reverse_iterator rbegin() const throw() { return const_reverse_iterator( this->dlist.tail ); } //!< begin reverse const
        const_reverse_iterator rend()   const throw() { return const_reverse_iterator(0);                  } //!< end reverse const


    private:
        mutable KEY_MAKER key_maker;
        inline const_type *find__( const_key_type &k ) const throw()
        {
            core::lw_key lwk = {0,0};
            key_maker(lwk,k);
            return this->search_(lwk.addr,lwk.size);
        }

    };

}

#endif

