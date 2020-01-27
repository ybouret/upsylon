
//! \file
#ifndef Y_HASHING_TYPE_MARK_INCLUDED
#define Y_HASHING_TYPE_MARK_INCLUDED 1

#include "y/hashing/fnv.hpp"
#include "y/type/mark.hpp"

namespace upsylon  {

    namespace hashing {

        //! dedicated hasher type for type mark
        template <typename HASH_FUNCTION = hashing::fnv >
        class type_mark_hasher
        {
        public:
            HASH_FUNCTION H;                             //!< generic hashing function
            inline   type_mark_hasher() throw() : H() {} //!< setup
            inline  ~type_mark_hasher() throw() {}       //!< cleanup

            //! return a hash of the name
            inline size_t operator()( const type_mark &mark ) throw()
            {
                return H.template key<size_t>(mark->name());
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(type_mark_hasher);
        };
    }
}

#endif

