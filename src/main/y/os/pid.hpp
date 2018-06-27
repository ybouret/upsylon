//! \file
#ifndef Y_OS_PID_INCLUDED
#define Y_OS_PID_INCLUDED 1

#include "y/os/platform.hpp"

#if defined(Y_BSD)
#    include <unistd.h>
#endif

namespace upsylon
{
    //! get the process id
    struct process_id
    {
        //! system dependent type
#if defined(Y_BSD)
        typedef pid_t   type;
#endif

#if defined(YOCTO_WIN)
        typedef uint32_t type;
#endif

        static type     get(); //!< get the PID
        static uint32_t h32(); //!< get the crc32 of it
    };


}
#endif

