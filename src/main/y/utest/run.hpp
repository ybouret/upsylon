//! \file
#ifndef Y_UTEST_RUN_INCLUDED
#define Y_UTEST_RUN_INCLUDED 1

#include "y/exception.hpp"
#include "y/alea.hpp"
#include <iostream>

//! test prolog
#define Y_UTEST(NAME)                                                 \
int upsylon_test_##NAME(int argc, char **argv) {                      \
(void)argc; (void)argv;                                               \
static const char   program[] = #NAME;   (void)program ;              \

//! test epilog
#define Y_UTEST_DONE() return 0; }

//! display and check
#define Y_CHECK(CODE) do {                                               \
const char _check[] = "CHECK [" #CODE "]";                               \
std::cerr << _check;                                                     \
if(!(CODE)) throw upsylon::exception(" [FAILED] <<" #CODE ">>");         \
for(int i=sizeof(_check)/sizeof(_check[0])-1;i<40;++i) std::cerr << ' '; \
std::cerr << "[PASSED]" << std::endl;                                    \
} while(false)

//! just assert
#define Y_ASSERT(CODE) do {                                     \
if(!(CODE)) throw upsylon::exception(" [FAILED] <<" #CODE ">>");   \
} while(false)

#endif

