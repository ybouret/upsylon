#include "y/ink/bitmap.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Ink;

Y_UTEST(bitmap)
{
    for(unit_t d=1;d<=16;++d)
    {
        for(size_t iter=1;iter<=2;++iter)
        {
            Bitmap bmp1( d, 1+alea.leq(100), 1+alea.leq(100));
            Bitmap bmp2( new Bitmap(d,1+alea.leq(100), 1+alea.leq(100)) );
        }
    }

}
Y_UTEST_DONE()


