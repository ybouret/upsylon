#include "y/ipso/dct.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(ipso_dct)
{
    for(unit_t w=1;w<=32;++w)
    {
        ipso::SquareDCT sq(w);
        for(unit_t h=1;h<=32;++h)
        {
            ipso::CommonDCT dct(w,h);
        }
    }
}
Y_UTEST_DONE()

