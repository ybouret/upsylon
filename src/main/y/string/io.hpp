
//! \file
#ifndef Y_STRING_IO_INCLUDED
#define Y_STRING_IO_INCLUDED 1

#include "y/string.hpp"
#include "y/ios/ostream.hpp"
#include "y/ios/istream.hpp"

namespace upsylon
{
    //! basic I/O for strings
    struct string_io
    {
        //static size_t save_binary(ios::ostream &fp, const string &s); //!< Length+Data
        static string load_binary(ios::istream &fp); //!< from Length+Data
        static ios::ostream & write_C(ios::ostream &fp, const string &s); //!< encode as C-string
    };
}

#endif
