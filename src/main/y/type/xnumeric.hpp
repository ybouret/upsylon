//! \file
#ifndef Y_XNUMERIC_INCLUDED
#define Y_XNUMERIC_INCLUDED 1

#include "y/type/ints.hpp"

namespace upsylon
{
    //! eXetended Numeric type
    template <typename T>
    struct xnumeric
    {
        //! absolute minimum for type
        static T abs_minimum();
    };
}


#endif

