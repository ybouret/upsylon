//! \file
#ifndef Y_STRING_TEMPORARY_NAME_INCLUDED
#define Y_STRING_TEMPORARY_NAME_INCLUDED 1

#include "y/string.hpp"
#include "y/container/sequence.hpp"

namespace upsylon
{
    //! handle temporary names
    struct temporary_name
    {
        //! make a random temporary file name '[length].tmp'
        static string create(const size_t length=8);
        
        //! append create a set of temporary file names '[length].tmp'
        static void   create(sequence<string> &names, const size_t count, const size_t length=8);
    };

}

#endif

