#include "y/gfx/bitmap/pixels.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace GFX;

Y_UTEST(pixels)
{

    for(size_t i=0;i<100000;i+=100+alea.leq(1000))
    {
        Pixels_ P(i);
        std::cerr << P << std::endl;
    }

}
Y_UTEST_DONE()

