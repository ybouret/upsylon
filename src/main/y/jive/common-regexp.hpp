
//! \file

#ifndef Y_JIVE_COMMON_REGEXP_INCLUDED
#define Y_JIVE_COMMON_REGEXP_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    namespace Jive
    {
        //______________________________________________________________________
        //
        //
        //! common regular expression
        //
        //______________________________________________________________________
        struct RegExpFor
        {
            static const char * const Identifier; //!< C identifier
            static const char * const Unsigned;   //!< unsigned value
            static const char * const Integer;    //!< integer value
            static const char * const Double;     //!< full double
            static const char * const Float;      //!< full float
        };
    }

}

#endif
