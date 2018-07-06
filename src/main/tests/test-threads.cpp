#include "y/concurrent/threads.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;


Y_UTEST(threads)
{
    std::cerr << "sizeof(thread)=" << sizeof(concurrent::thread) << std::endl;
    concurrent::threads threads(true);

    threads.synchronize.broadcast();
    

}
Y_UTEST_DONE()
