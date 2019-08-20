//! \file
#ifndef Y_STRING_TEMPORARY_NAME_INCLUDED
#define Y_STRING_TEMPORARY_NAME_INCLUDED 1

#include "y/string.hpp"
#include "y/container/sequence.hpp"

namespace upsylon
{
    struct temporary_name
    {
        static string create(const size_t length=8); //!< make a random temporary file name
        static void   create(sequence<string> &names, const size_t count, const size_t length=8); //!< append count different names
    };

}

#endif

