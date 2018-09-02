#include "y/ink/ops/blob.hpp"
#include "y/ink/image.hpp"
#include "y/ink/ops/histogram.hpp"
#include "y/utest/run.hpp"
#include "y/ink/color/named-colors.hpp"

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
        const size_t  level = Histogram::Level(img3,RGBtoByte,E);
        Threshold::Foreground(fg,img3,RGBtoByte,level,E,Pixel<RGB>::Copy);

        img.save("img3.png",img3,0);
        img.save("fg.png",fg,0);

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

