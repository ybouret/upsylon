#include "y/concurrent/threads.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;


Y_UTEST(threads)
{
    std::cerr << "sizeof(thread)=" << sizeof(concurrent::thread) << std::endl;
    size_t n = 2;
    if(argc>1)
    {
        n = atol(argv[1]);
    }
    concurrent::threads threads(n,true);


}
Y_UTEST_DONE()
