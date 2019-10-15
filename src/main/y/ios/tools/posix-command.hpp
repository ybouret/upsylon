//! \file

#ifndef Y_POSIX_COMMAND_INCLUDED
#define Y_POSIX_COMMAND_INCLUDED 1

#include "y/strfwd.hpp"
#include "y/container/sequence.hpp"

namespace upsylon{

    namespace ios
    {
        struct posix_command
        {
            static void query( sequence<string> &lines, const string &cmd );
        };
    }
}

#endif

