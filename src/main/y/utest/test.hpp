#ifndef Y_UTEST_PROC_INCLUDED
#define Y_UTEST_PROC_INCLUDED 1

#include "y/exception.hpp"
#include <iostream>

struct upsylon_test
{
    typedef int (*proc_type)(int argc, char **argv);
    proc_type   proc;
    const char *name;
};
#endif

