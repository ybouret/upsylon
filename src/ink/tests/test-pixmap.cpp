
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
        arc_ptr<bitmap> bmp1 = img.load1(fn,NULL);

    }

}
Y_UTEST_DONE()

