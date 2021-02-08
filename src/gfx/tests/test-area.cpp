
#include "y/gfx/area.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace GFX;

Y_UTEST(area)
{
    Area a(0,0,10,20);    std::cerr << "area = " << a << std::endl;
    Area c = a.getCore(); std::cerr << "core = " << c << std::endl;
}
Y_UTEST_DONE()

