
//! \file

#ifndef Y_XFACTORY_INCLUDED
#define Y_XFACTORY_INCLUDED 1

#include "y/container/factory.hpp"
#include "y/string.hpp"

namespace upsylon {

    template <
    typename T,
    typename CREATOR    = T (*)(),
    typename KEY_HASHER = key_hasher<string>,
    typename ALLOCATOR  = memory::global>
    class xfactory : public factory<T,string,CREATOR,KEY_HASHER,ALLOCATOR>
    {
    public:
        typedef factory<T,string,CREATOR,KEY_HASHER,ALLOCATOR> factory_type;

        inline virtual ~xfactory() throw() {}
        inline explicit xfactory() throw() : factory_type() {}

        inline void decl(const char *key, const CREATOR &creator )
        {
            const string _(key); factory_type::declare(_,creator);
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(xfactory);
    };

}

#endif

