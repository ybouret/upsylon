
//! \file
#ifndef Y_CODE_TEXTUAL_INCLUDED
#define Y_CODE_TEXTUAL_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{
    //! textual helpers
    struct textual
    {
        //! return "s" if count>1, "" otherwise
        static const char *plural_s(const size_t count) throw();

        //! return "a" if count>1, "um" otherwise
        static const char *plural_a(const size_t count) throw();

        //! return true or false
        static const char *boolean(const bool flag)     throw();
    };



}

#endif

