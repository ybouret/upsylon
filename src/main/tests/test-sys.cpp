
#include "y/os/hw.hpp"
#include "y/os/wtime.hpp"
#include "y/utest/timings.hpp"
#include "y/utest/run.hpp"
#include "y/utest/timings.hpp"

using namespace upsylon;

Y_UTEST(sys)
{
    const size_t np = hardware::nprocs();
    std::cerr << "-- #logical processors=" << np << std::endl;
    Y_CHECK(hardware::nprocs()>0);

    std::cerr << "-- wall time" << std::endl;
    wtime chrono;
    for(size_t i=0;i<10;++i)
    {
        std::cerr << "#ticks=" << rt_clock::ticks() << std::endl;
        std::cerr << "chrono=" << chrono.query() << std::endl;
        chrono.sleep(0.02);
    }

    std::cerr << "-- timings" << std::endl;
    char   buffer[1000];
    double tmx=0;
    Y_TIMINGS(tmx,1.0,crc32(buffer,sizeof(buffer)));
    std::cerr << "tmx=" << tmx << "/s" << std::endl;
}
Y_UTEST_DONE()

