//! \file
#ifndef Y_OS_PID_INCLUDED
#define Y_OS_PID_INCLUDED 1

#include "y/os/platform.hpp"

#if defined(Y_BSD)
#    include <unistd.h>
#endif

namespace upsylon
{
    struct process_id
    {
#if defined(Y_BSD)
        typedef pid_t   type;
#endif

#if defined(YOCTO_WIN)
        typedef uint32_t type;
#endif

        static type     get();
        static uint32_t h32();
    };


}
#endif

