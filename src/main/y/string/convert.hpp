//! \file
#ifndef Y_STRING_CONVERT_INCLUDED
#define Y_STRING_CONVERT_INCLUDED 1

#include "y/string.hpp"

namespace upsylon
{
    struct string_convert
    {
        template <typename T>
        T to( const string &data, const char *name);
    };
}

#endif

