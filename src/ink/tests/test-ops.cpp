#include "y/ink/ops/filter.hpp"
#include "y/ink/image.hpp"
#include "y/utest/run.hpp"
#include "y/ink/color/convert.hpp"

using namespace upsylon;
using namespace ink;

Y_UTEST(ops)
{
    imageIO   &img   = image::init();
    dispatcher __par = new concurrent::simd();

    matrix<int> M(3,3);

    for(int iarg=1;iarg<argc;++iarg)
    {
        const string fn  = argv[iarg];
        pixmap3      pxm = img.load3(fn,NULL);

        const size_t w = pxm.w;
        const size_t h = pxm.h;
        engine       par(__par,pxm);

        pixmap1      pxm1(w,h);
        filter::apply(pxm1,pxm,color_convert::make<uint8_t,RGB>,par);
        img.save("img1.png",pxm1,0);

        pixmapf target(w,h);

        filter::stencil(target,pxm1,M,par);
        //img.save("stenf.png",pxmf,0);
        

        pixmaps<uint8_t> ch3(3,w,h);
        

    }

}
Y_UTEST_DONE()

