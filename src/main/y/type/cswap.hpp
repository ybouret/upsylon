// \file
#ifndef Y_TYPE_CSWAP_INCLUDED
#define Y_TYPE_CSWAP_INCLUDED 1

#include "y/os/platform.hpp"
namespace upsylon
{
    template <typename T>
    inline void cswap( T &a, T &b )
    {
        const T tmp(a);
        a=b;
        b=tmp;
    }

  
}

#endif

