#ifndef Y_UTEST_RUN_INCLUDED
#define Y_UTEST_RUN_INCLUDED 1

#include "y/utest/test.hpp"

#define Y_UTEST(NAME) \
int upsylon_test_##NAME(int argc, char *argv[]) {\
(void)argc; (void)argv;

#define Y_UTEST_DONE() return 0; }

#define Y_CHECK(CODE) do {\
if(!(CODE)) throw upsylon::exception("FAILED " #CODE);\
} while(false)

#endif

