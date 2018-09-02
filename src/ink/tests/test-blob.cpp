#include "y/ink/ops/blob.hpp"
#include "y/ink/image.hpp"
#include "y/ink/ops/histogram.hpp"
#include "y/utest/run.hpp"
#include "y/ink/color/named-colors.hpp"
#include "y/ink/ops/filter.hpp"

using namespace upsylon;
using namespace Ink;

Y_UTEST(blob)
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
        Pixmap3       fg(w,h);
        Pixmap3       fg2(w,h);
        const size_t  level = Histogram::Level(img3,RGBtoByte,E);
        Threshold::Foreground(fg,img3,RGBtoByte,level,E,Pixel<RGB>::Copy);

        img.save("img3.png",img3,0);
        img.save("fg0.png",fg,0);

        fg2.copy(fg,E);
        Filter::FillHoles(fg,E);
        img.save("fg1.png",fg,0);

        Filter::FillHoles(fg2,img3,E);
        img.save("fg2.png",fg2,0);

        Filter::Average(fg2,fg,E);
        img.save("ave.png",fg2,0);

        Filter::Median(fg2,fg,E);
        img.save("med.png",fg2,0);

        Filter::Erode(fg2,fg,E);
        img.save("erode.png",fg2,0);

        Filter::Dilate(fg2,fg,E);
        img.save("dilate.png",fg2,0);

        Blobs blobs(w,h);

        Blob::List particles;

        blobs.build(particles,fg,Connect4);
        
        index_to_rgba proc;
        img.save("blob4.png", blobs, proc, 0);
        
        blobs.build(particles,fg,Connect8);
        img.save("blob8.png", blobs, proc, 0);

        
    }


}
Y_UTEST_DONE()

