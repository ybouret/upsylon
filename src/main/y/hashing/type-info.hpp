
//! \file

#ifndef Y_HASHING_TYPE_INFO_INCLUDED
#define Y_HASHING_TYPE_INFO_INCLUDED 1

#include "y/hashing/fnv.hpp"

namespace upsylon
{
    namespace hashing
    {
        //______________________________________________________________________
        //
        //! direct hashing of type_info name
        //______________________________________________________________________
        template <typename HASH_FUNCTION>
        class type_info_hasher
        {
        public:
            inline explicit type_info_hasher() throw() {} //!< setup
            inline virtual ~type_info_hasher() throw() {} //!< cleanup
            
            //! use name as text
            inline size_t operator()(const std::type_info &tid) const throw()
            {
                return HASH_FUNCTION::of(tid.name());
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(type_info_hasher);
        };
    }
}

#endif
