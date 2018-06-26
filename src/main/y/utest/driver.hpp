#ifndef Y_UTEST_DRIVER_INCLUDED
#define Y_UTEST_DRIVER_INCLUDED 1

#include "y/utest/proc.hpp"

#define Y_UTEST_INIT(N) \
int main(int argc, char **argv) {\
try {\
upsylon_test tests[N];\
size_t       ntest=0;

#define Y_UTEST_EXEC()\
if(argc<=1) {\
for(size_t i=0;i<ntest;++i) { std::cout << tests[i].name << std::endl; }\
}\
}\
catch( upsylon::exception &e ) { \
std::cerr << e.what() << std::endl;\
std::cerr << e.when() << std::endl;\
return 1;\
}\
catch( std::exception &e ) { \
std::cerr << e.what() << std::endl;\
return 2;\
}\
catch(...) {\
std::cerr << "unhandled exception" << std::endl;\
return 3;\
}\
return 0;\
}

#endif


