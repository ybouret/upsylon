//! \file

#ifndef Y_POSIX_COMMAND_INCLUDED
#define Y_POSIX_COMMAND_INCLUDED 1

#include "y/strfwd.hpp"
#include "y/container/sequence.hpp"

namespace upsylon{

    namespace ios
    {
        //! execute a system() call with redirection and return content
        struct posix_command
        {
            //! system( cmd &> tmp ) and get line
            static void query( sequence<string> &lines, const string &cmd );
        };
    }
}

#endif

