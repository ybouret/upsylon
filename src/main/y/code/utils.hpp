//! \file
#ifndef Y_CODE_UTILS_INCLUDED
#define Y_CODE_UTILS_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{

    //! return "s" if count>1
    inline const char * plural_s(const size_t count) throw()
    {
        return (count<=1) ? "" : "s";
    }

    struct hexadecimal
    {
        static const char *lowercase_word[16];
        static const char *uppercase_word[16];
        static const char *lowercase[256];
        static const char *uppercase[256];

    };
}

#endif

