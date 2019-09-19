//! \file

#ifndef Y_BTABLE_INCLUDED
#define Y_BTABLE_INCLUDED 1

#include "y/associative/btree.hpp"
#include "y/type/class-conversion.hpp"

namespace upsylon
{

    namespace core
    {
        struct lw_key
        {
            const void *addr;
            size_t      size;
        };

        template <typename T>
        class lw_key_maker
        {
        public:
            Y_DECL_ARGS(T,type);      //!< alias
            
            inline  lw_key_maker() throw() {}
            inline ~lw_key_maker() throw() {}

            inline void operator()(lw_key &lwk, param_type key ) const throw()
            {
                 return fill(lwk, key, int2type< Y_IS_SUPERSUBCLASS(memory::ro_buffer,mutable_type) >() );
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(lw_key_maker);
            void fill(lw_key &lwk, param_type key, int2type<true> ) const throw()
            {
                lwk.addr = key.ro();
                lwk.size = key.length();
            }

            void fill(lw_key &lwk, param_type key, int2type<false> ) const throw()
            {
                lwk.addr = &key;
                lwk.size = sizeof(type);
            }


        };
    }



    template <
    typename KEY,
    typename T,
    typename KEY_MAKER = core::lw_key_maker<KEY>
    >
    class btable : public btree<T>, public associative<KEY,T>
    {
    public:
        Y_DECL_ARGS(KEY,key_type); //!< alias
        Y_DECL_ARGS(T,type);       //!< alias

        inline explicit btable() throw() : btree<T>()
        {
        }

        inline virtual ~btable() throw()
        {
        }
        
        inline virtual size_t size() const throw()     { return this->count; }
        inline virtual void   free()       throw()     { this->free_();      }
        inline virtual void   release()    throw()     { this->release_();   }
        inline virtual size_t capacity() const throw() { return this->nodes; }
        inline virtual void   reserve(const size_t n)  { this->reserve_(n);  }

        inline virtual bool remove( param_key_type k ) throw()
        {
            return false;
        }

        virtual type *search( param_key_type k ) throw()
        {
            return (type *)find__(k);
        }

        virtual const_type *search( param_key_type k ) const throw()
        {
            return  find__(k);

        }



    private:
        Y_DISABLE_COPY_AND_ASSIGN(btable);
        mutable KEY_MAKER key_maker;
        inline const_type *find__( param_key_type k ) const throw()
        {
            core::lw_key lwk = {0,0};
            key_maker(lwk,k);
            return this->search_(lwk.addr,lwk.size);
        }

    };

}

#endif

