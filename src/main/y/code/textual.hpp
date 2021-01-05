
//! \file
#ifndef Y_CODE_TEXTUAL_INCLUDED
#define Y_CODE_TEXTUAL_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{
    //! textual helpers
    struct textual
    {
        //! return "s" if count>1
        static const char *plural_s(const size_t count) throw();

        //! return true or false
        static const char *boolean(const bool flag)     throw();
    };



}

#endif

