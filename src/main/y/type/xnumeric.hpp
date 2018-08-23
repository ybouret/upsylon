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
        //! test if zero
        static inline bool is_zero( const T &x )    { return x==0; }
        //! test if greater than zero
        static inline bool is_positive( const T &x) { return x>0;  }
    };
}


#endif

