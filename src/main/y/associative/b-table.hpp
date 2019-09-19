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
    class btable : public btree<T>
    {
    public:
        explicit btable() throw() : btree<T>()
        {
        }

        virtual ~btable() throw()
        {
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(btable);
        KEY_MAKER key_maker;
    };

}

#endif

