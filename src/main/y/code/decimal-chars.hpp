
//! \file

#ifndef Y_DECIMAL_CHARS_INCLUDED
#define Y_DECIMAL_CHARS_INCLUDED

#include "y/os/platform.hpp"

namespace upsylon
{

    //! decimal output chars for an unsigned type
    template <typename T> inline
    size_t decimal_chars_for(T u) throw()
    {
        if(u<=9)
        {
            return 1;
        }
        else
        {
            size_t n = 1;
            while( (u/=10) > 0 ) ++n;
            return n;
        }
    }
}

#endif
