
#include "y/ink/image.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include <typeinfo>
//#include "y/ptr/auto.hpp"
#include "y/ptr/arc.hpp"

using namespace upsylon;
using namespace ink;

namespace
{

}

Y_UTEST(pixmap)
{
    imageIO &img = image::instance().initialize();

    for(int iarg=1;iarg<argc;++iarg)
    {
        const string    fn   = argv[iarg];
        pixmapf         pxmf = img.loadf(fn,0);
        pixmap1         pxm1 = img.load1(fn,0);
        pixmap3         pxm3 = img.load3(fn,0);
        pixmap4         pxm4 = img.load4(fn,0);

    }

}
Y_UTEST_DONE()

