
#include "y/os/hw.hpp"
#include "y/os/rt-clock.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(sys)
{
    const size_t np = hardware::nprocs();
    std::cerr << "#logical processors=" << np << std::endl;
    Y_CHECK(hardware::nprocs()>0);
    for(size_t i=0;i<10;++i)
    {
        std::cerr << "#ticks=" << rt_clock::ticks() << std::endl;
    }
    
}
Y_UTEST_DONE()

