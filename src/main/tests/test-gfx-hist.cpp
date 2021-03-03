
#include "y/graphic/color/histogram.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace graphic;

Y_UTEST(gfx_hist)
{
    histogram H;
    {
        const float f = 0.2f;
        H << f;
    }

    {
        const rgb c(10,20,30);
        H << c;
    }


}
Y_UTEST_DONE()
