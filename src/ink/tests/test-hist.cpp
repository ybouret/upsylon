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
        const size_t  w    = img3.w;
        const size_t  h    = img3.h;
        Engine        E(par,img3);
        PixmapF       imgf(w,h);
        Filter::Apply(imgf,img3,RGBtoFloat,E);

        img.save("img3.png",img3,0);
        img.save("imgf.png",imgf,0);


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

        const size_t level = H3.threshold();
        std::cerr << "level=" << level << std::endl;

        Pixmap3 tgt3(w,h);

        Threshold::Foreground(tgt3,img3,RGBtoByte,level,E);
        img.save("fg_normal.png",tgt3,0);
        Threshold::Foreground(tgt3,img3,RGBtoByte,level,E,true);
        img.save("fg_strict.png",tgt3,0);

        Threshold::Background(tgt3,img3,RGBtoByte,level,E);
        img.save("bg_normal.png",tgt3,0);
        Threshold::Background(tgt3,img3,RGBtoByte,level,E,true);
        img.save("bg_strict.png",tgt3,0);

        Threshold::InverseBackground(tgt3,img3,RGBtoByte,level,E);
        img.save("ib_normal.png",tgt3,0);
        Threshold::InverseBackground(tgt3,img3,RGBtoByte,level,E,true);
        img.save("ib_strict.png",tgt3,0);

    }
}
Y_UTEST_DONE()

