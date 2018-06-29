//! \file
#ifndef Y_OS_HW_INCLUDED
#define Y_OS_HW_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{

    //! gather hardware information
    struct hardware
    {
        static size_t nprocs(); //!< number of logical processors
    };

}


#endif
