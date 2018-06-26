// \file
#ifndef Y_UTEST_RUN_INCLUDED
#define Y_UTEST_RUN_INCLUDED 1

#include "y/exception.hpp"
#include <iostream>

#define Y_UTEST(NAME) \
int upsylon_test_##NAME(int argc, char **argv) {\
(void)argc; (void)argv;

#define Y_UTEST_DONE() return 0; }

#define Y_CHECK(CODE) do {\
std::cerr << "CHECK: " #CODE << std::endl;\
if(!(CODE)) throw upsylon::exception("FAILED " #CODE);\
} while(false)

#endif

