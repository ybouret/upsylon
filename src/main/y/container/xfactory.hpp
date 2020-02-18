
//! \file

#ifndef Y_XFACTORY_INCLUDED
#define Y_XFACTORY_INCLUDED 1

#include "y/container/factory.hpp"
#include "y/string.hpp"

namespace upsylon {

    template <
    typename T,
    typename CREATOR    = T * (*)(),
    typename KEY_HASHER = key_hasher<string>,
    typename ALLOCATOR  = memory::global>
    class xfactory : public factory<T,string,CREATOR,KEY_HASHER,ALLOCATOR>
    {
    public:
        typedef factory<T,string,CREATOR,KEY_HASHER,ALLOCATOR> factory_type;

        inline virtual ~xfactory() throw() {}

        inline explicit xfactory() throw() : factory_type() {}

        inline explicit xfactory(const size_t n) : factory_type(n) {}

        inline void use(const string &key, const CREATOR &creator )
        {
            factory_type::declare(key,creator);
        }

        inline void use(const char *key, const CREATOR &creator )
        {
            const string _(key); use(_,creator);
        }

        inline const CREATOR *query( const string &key ) const throw()
        {
            return factory_type::look_for(key);
        }

        inline const CREATOR *query( const char *key ) const throw()
        {
            const string _(key); return query(_);
        }

        inline T * operator()( const string &key ) const
        {
            return factory_type::create(key);
        }

        inline T * operator()( const char *key ) const
        {
            const string _(key); return (*this)(_);
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(xfactory);
    };

}

#endif

