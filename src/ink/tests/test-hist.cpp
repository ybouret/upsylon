#include "y/ink/ops/histogram.hpp"
#include "y/utest/run.hpp"
#include "y/ink/image.hpp"
#include "y/ink/ops/filter.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace Ink;

Y_UTEST(hist)
{
    ImageIO   &img = Image::Init();
    Dispatcher par = new concurrent::simd();

    if(argc>1)
    {
        const string  fn   = argv[1];
        const Pixmap3 img3 = img.load3(fn,0);
        const size_t  w = img3.w;
        const size_t  h = img3.h;
        Engine        E(par,img3);
        PixmapF       imgf(w,h);
        Filter::Apply(imgf,img3,RGBtoFloat,E);

        Histogram H3;
        H3.build(img3,RGBtoByte,E);
        Histogram HF;
        HF.build(imgf,Crux::FloatToByte,E);
        {
            ios::ocstream fp("hist.dat");
            for(unsigned i=0;i<Histogram::BINS;++i)
            {
                fp("%u %u %u\n",i,unsigned(H3.bins[i]),unsigned(HF.bins[i]));
            }
        }


    }
}
Y_UTEST_DONE()

