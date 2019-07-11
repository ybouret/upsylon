//! \file
#ifndef Y_STRING_DISPLAY_INCLUDED
#define Y_STRING_DISPLAY_INCLUDED 1

#include "y/string.hpp"

namespace upsylon
{

    //! utilities to display strings
    struct string_display
    {
        //! output an aligned block
        static std::ostream &  align( std::ostream &os, const char   *s, const size_t n, const size_t width );
        //! output an aligned text
        static std::ostream &  align( std::ostream &os, const char   *s, const size_t width );
        //! output and aligned stringstring_display:: align
        static std::ostream &  align( std::ostream &os, const string &s, const size_t width );
    };
}

#endif

