
#include "y/os/hw.hpp"
#include "y/os/wtime.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(sys)
{
    const size_t np = hardware::nprocs();
    std::cerr << "#logical processors=" << np << std::endl;
    Y_CHECK(hardware::nprocs()>0);

    wtime chrono;
    for(size_t i=0;i<10;++i)
    {
        std::cerr << "#ticks=" << rt_clock::ticks() << std::endl;
        std::cerr << "chrono=" << chrono.query() << std::endl;
        chrono.sleep(0.1);
    }
    
}
Y_UTEST_DONE()

