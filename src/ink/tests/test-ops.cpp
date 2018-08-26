#include "y/ink/ops/filter.hpp"
#include "y/ink/image.hpp"
#include "y/utest/run.hpp"
#include "y/ink/color/convert.hpp"

using namespace upsylon;
using namespace ink;

Y_UTEST(ops)
{
    imageIO   &img = image::init();
    dispatcher __par = new concurrent::simd();

    for(int iarg=1;iarg<argc;++iarg)
    {
        const string fn  = argv[iarg];
        pixmap3      pxm = img.load3(fn,NULL);

        const size_t w = pxm.w;
        const size_t h = pxm.h;
        engine       par(__par,pxm);

        pixmapf      pxmf(w,h);
        filter::apply(pxmf,pxm,color_convert::make<float,RGB>,par);

        pixmaps<uint8_t> ch3(3,w,h);
        

    }

}
Y_UTEST_DONE()

