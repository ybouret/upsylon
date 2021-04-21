//! \file

#ifndef Y_CONCURRENT_ASSIGN_MAIN_INCLUDED
#define Y_CONCURRENT_ASSIGN_MAIN_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{
    //__________________________________________________________________________
    //
    //
    //! helper to assign main thread
    //
    //__________________________________________________________________________
    struct assign_main
    {
        static size_t max_cpus();                            //!< hardware::nprocs
        static void   to(const size_t cpu, const char *who); //!< wrapper
        static size_t by_pid(const char *who);               //!< try to dispatch from pid
    };
}

#endif
