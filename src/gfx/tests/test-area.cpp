
#include "y/gfx/area.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace GFX;

Y_UTEST(area)
{
    Area a(0,0,10,20);

    std::cerr << a << std::endl;

}
Y_UTEST_DONE()

