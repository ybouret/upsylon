//! \file
#ifndef Y_KEY_HASHER_INCLUDED
#define Y_KEY_HASHER_INCLUDED 1


#include "y/hashing/fnv.hpp"
#include "y/hashing/sha1.hpp"
#include "y/type/args.hpp"
#include "y/type/class-conversion.hpp"
#include "y/memory/buffer.hpp"

namespace upsylon
{

    //! generic key hasher return size_t from any type
    template <typename T, typename HASHING_FUNCTION = hashing::fnv >
    class key_hasher
    {
    public:
        Y_DECL_ARGS(T,type);      //!< alias
        HASHING_FUNCTION  __hash; //!< hashing function for external use

        //! constructor
        inline  key_hasher() : __hash() {}

        //! desctructor
        inline ~key_hasher() throw() {}

        //! hasher invocation
        inline size_t operator()( param_type args ) throw()
        {
            return hash( args, int2type< Y_IS_SUPERSUBCLASS(memory::ro_buffer,mutable_type) >() );
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(key_hasher);

        inline size_t hash( param_type args, int2type<true> ) throw()
        {
            return __hash.template key<size_t>( args.ro(), args.length() );
        }

        inline size_t hash( param_type args, int2type<false> ) throw()
        {
            return __hash.template key<size_t>( &args, sizeof(type) );
        }

    };

}
#endif

