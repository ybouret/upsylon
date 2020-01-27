//! \file
#ifndef Y_HASHING_TYPE_SPEC_INCLUDED
#define Y_HASHING_TYPE_SPEC_INCLUDED 1

#include "y/hashing/fnv.hpp"
#include "y/type/spec.hpp"

namespace upsylon {

    namespace hashing {

        //! dedicated hasher type for type_spec
        template <typename HASH_FUNCTION = hashing::fnv >
        class type_spec_hasher
        {
        public:
            HASH_FUNCTION H;                             //!< generic hashing function
            inline   type_spec_hasher() throw() : H() {} //!< setup
            inline  ~type_spec_hasher() throw() {}       //!< cleanup

            //! return a hash of the name
            inline size_t operator()( const type_spec &ts ) throw()
            {
                return H.template key<size_t>(ts.uuid);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(type_spec_hasher);
        };
    }
}

#endif
