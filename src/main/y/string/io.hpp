
//! \file
#ifndef Y_STRING_IO_INCLUDED
#define Y_STRING_IO_INCLUDED 1

#include "y/string.hpp"
#include "y/ios/ostream.hpp"
#include "y/ios/istream.hpp"

namespace upsylon
{
    struct string_io
    {
        static void   save_binary(ios::ostream &fp, const string &s);
        static string load_binary(ios::istream &fp);
    };
}

#endif
