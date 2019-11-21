
#include "y/graphic/types.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Graphic;

Y_UTEST(types)
{

    size_t  num = 5;
    std::cerr << "required objects=" << num << std::endl;
    double *arr = MemoryAcquireAs<double>(num);
    std::cerr << "allocated bytes=" << num << std::endl;
    MemoryReleaseAs(arr,num);
    Y_CHECK(0==num);
    Y_CHECK(0==arr);


}
Y_UTEST_DONE()

