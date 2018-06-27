// \file
#ifndef Y_AT_EXIT_INCLUDED
#define Y_AT_EXIT_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{

    struct at_exit
    {
        static const size_t stack_size = 64;
        typedef void (*procedure)(void *);
        typedef int    longevity;
        static void perform( procedure proc, void *args, const longevity when) throw();
    };
}

#endif

