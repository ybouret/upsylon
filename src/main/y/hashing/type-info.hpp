//! \file
#ifndef Y_HASHING_TYPE_INFO_INCLUDED
#define Y_HASHING_TYPE_INFO_INCLUDED 1

#include "y/hashing/fnv.hpp"
#include <typeinfo>

namespace upsylon
{
    namespace hashing
    {
     
        template <typename HASH_FUNCTION = hashing::fnv >
        class type_info_hasher
        {
        public:
            HASH_FUNCTION H;
            
            inline   type_info_hasher() throw() : H() {}
            inline  ~type_info_hasher() throw() {}
            
            inline size_t operator()( const std::type_info &tid ) throw()
            {
                return H.template key<size_t>(tid.name());
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(type_info_hasher);
        };
    }
}

#endif

