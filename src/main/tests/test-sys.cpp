
#include "y/os/hw.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(sys)
{
    const size_t np = hardware::nprocs();
    std::cerr << "#logical processors=" << np << std::endl;
    Y_CHECK(hardware::nprocs()>0);
}
Y_UTEST_DONE()

