#include "y/ink/bitmap.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace ink;

Y_UTEST(bitmap)
{
    for(unit_t d=1;d<=16;++d)
    {
        for(size_t iter=1;iter<=2;++iter)
        {
            bitmap bmp1( d, 1+alea.leq(100), 1+alea.leq(100));
            bitmap bmp2( new bitmap(d,1+alea.leq(100), 1+alea.leq(100)) );
        }
    }

}
Y_UTEST_DONE()


