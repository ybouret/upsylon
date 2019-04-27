
//! \file
#ifndef Y_TYPE_INTS_CHKBND_INCLUDED
#define Y_TYPE_INTS_CHKBND_INCLUDED 1

#include "y/type/ints.hpp"

#include <iostream>

namespace upsylon
{

    
    //! return a positive bounded value of OUPUT based on INPUT type
    template <typename OUTPUT,typename INPUT>
    inline OUTPUT check_bound( INPUT x ) throw()
    {

        static const uint64_t  omax64 = static_cast<uint64_t>(limit_of<OUTPUT>::maximum);
        static const uint64_t  imax64 = static_cast<uint64_t>(limit_of<INPUT> ::maximum);
        static const uint64_t  vmax64 = (omax64<imax64) ? omax64 : imax64;
        static const INPUT     imax   = static_cast<INPUT>(vmax64);

        x = limit_of<INPUT>::positive(x);
        if( static_cast<uint64_t>(x) > vmax64 )
        {
            return imax;
        }
        else
        {
            return x;
        }
    }

    
}

#endif

