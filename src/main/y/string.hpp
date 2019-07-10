//! \file
#ifndef Y_STRING_INCLUDED
#define Y_STRING_INCLUDED 1

#include "y/string/basic.hpp"
#include "y/os/printf-check.hpp"

namespace upsylon
{
    //! string class
    typedef core::string<char> string;

    //! make a string from printf like call
    string vformat( const char *fmt,... ) Y_PRINTF_CHECK(1,2);

    std::ostream & display_align( std::ostream &os, const char   *s, const size_t n, const size_t width );
    std::ostream & display_align( std::ostream &os, const char   *s, const size_t width );
    std::ostream & display_align( std::ostream &os, const string &s, const size_t width );


}

#endif
