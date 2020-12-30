
//! \file
#ifndef Y_VECTORS_INCLUDED
#define Y_VECTORS_INCLUDED 1

#include "y/sequence/vector.hpp"

namespace upsylon
{

    //! wrapper to compute a specific vector type
    template <typename ALLOCATOR = memory::global >
    struct __vector
    {
        //! deduce type
        template <typename T>
        struct of
        {
            typedef vector<T,ALLOCATOR> type; //!< the type
        };
    };

}

#endif
