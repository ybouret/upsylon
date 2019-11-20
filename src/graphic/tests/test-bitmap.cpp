
#include "y/graphic/bitmap.hpp"
#include "y/utest/run.hpp"
#include "y/type/bzset.hpp"

using namespace upsylon;
using namespace Graphic;

Y_UTEST(bitmap)
{
    Metrics mtx;
    bzset(mtx);
    mtx.width  = 1 + alea.leq(100);
    mtx.height = 1 + alea.leq(100);
    mtx.depth  = 1 << ( alea.leq(3));
    mtx.stride = mtx.width * mtx.depth;
    mtx.count  = mtx.stride * mtx.height;
    mtx.entry  = Metrics::Acquire(mtx.count);
    mtx.flags  = Metrics::GlobalMemory | Metrics::ReadWrite;

    Bitmap bmp( mtx );

}
Y_UTEST_DONE()
