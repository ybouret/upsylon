//! \file

#ifndef Y_STRING_TIME_INCLUDED
#define Y_STRING_TIME_INCLUDED 1

#include "y/string.hpp"

namespace upsylon
{
    //__________________________________________________________________________
    //
    //! various time formating routines
    //__________________________________________________________________________
    struct string_time
    {
        static string stamp(); //!< "y-m-d H:M:S" stamp
    };
}

#endif
