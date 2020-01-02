
#include "y/graphic/ops/blur.hpp"

#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "support.hpp"
#include "y/graphic/image.hpp"
#include "y/graphic/pixmaps.hpp"
#include "y/concurrent/scheme/simd.hpp"

using namespace upsylon;
using namespace Graphic;

Y_UTEST(blur)
{
    if(argc>1)
    {
        const string        filename = argv[1];
        Image              &IMG      = Image::instance();
        const Pixmap4       pxm4( IMG.loadAs<rgba>(filename) );

    }
}
Y_UTEST_DONE()
