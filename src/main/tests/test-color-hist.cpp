
#include "y/color/histogram.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(color_hist)
{
    color_histogram H;
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
