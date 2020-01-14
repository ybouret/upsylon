//! \file
#ifndef Y_TYPE_CSWAP_INCLUDED
#define Y_TYPE_CSWAP_INCLUDED 1

#include "y/os/platform.hpp"
namespace upsylon
{
    //! C-style swap
    template <typename T>
    inline void cswap( T &a, T &b )
    {
        const T tmp(a);
        a=b;
        b=tmp;
    }

    //! C-style forced swap, DANGEROUS
    template <typename T>
    inline void _cswap( const T &a, const T &b )
    {
        const T tmp(a);
        (T&)a=b;
        (T&)b=tmp;
    }

  
}

#endif

