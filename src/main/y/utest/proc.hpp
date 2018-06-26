#ifndef Y_UTEST_PROC_INCLUDED
#define Y_UTEST_PROC_INCLUDED 1

#include "y/exception.hpp"
#include <iostream>

struct upsylon_test
{
    typedef int (*proc)(int argc, char **argv);
    proc        p;
    const char *n;
};
#endif

