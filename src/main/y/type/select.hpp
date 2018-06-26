//! \file
#ifndef Y_TYPE_SELECT_INCLUDED
#define Y_TYPE_SELECT_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{
    //! default type selector.
    template <const bool flag, typename T, typename U> struct select;
    
    //! type selector when true => T
    template <typename T, typename U>
    struct select<true,T,U>
    {
        typedef T result; //!< flag is true
    };
    
    //! type selector when false => U
    template <typename T, typename U>
    struct select<false,T,U>
    {
        typedef U result; //!< flag is false
    };
}

#endif

