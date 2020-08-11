//! \file
#ifndef Y_AT_EXIT_INCLUDED
#define Y_AT_EXIT_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{

    //! wrapper for libc atexit
    struct at_exit
    {
        static  const size_t stack_size = 64;   //!< maximum number of register procedures
        typedef void       (*procedure)();      //!< a procedure to be called
        typedef int16_t      longevity;         //!< its longevity
        //! register a new procedure
        /**
         The procedures are stored in a FIFO order (like atexit) for the
         same longevity, but executed by increasing longevity otherwise
         */
        static void perform( procedure proc, const longevity when) throw();
    };
}

#endif

