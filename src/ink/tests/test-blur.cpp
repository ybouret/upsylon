#include "y/ink/ops/blur.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Ink;

Y_UTEST(blur)
{
    for(float sig=0.1;sig<=5.0;sig+=0.1)
    {
        const Blur blur(sig);
        std::cerr << "blur.sigma=" << blur.sigma << " -> length=" << blur.length << std::endl;
        std::cerr << "|_" << blur.weight << std::endl;
    }
}
Y_UTEST_DONE()


