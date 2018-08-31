#include "y/ink/ops/blur.hpp"
#include "y/utest/run.hpp"
#include "y/ink/image.hpp"

using namespace upsylon;
using namespace Ink;

Y_UTEST(blur)
{
    for(float sig=0.1;sig<=5.0;sig+=0.1)
    {
        const Blur blur(sig);
        std::cerr << "blur.sigma=" << blur.sigma << "#=" << blur.length << blur.weight << std::endl;
        if(blur.length<10)
        {
            std::cerr << "norm=" << blur.normal << std::endl;
        }
    }


    ImageIO   &img = Image::Init();
    Dispatcher par = new concurrent::simd();

    if(argc>1)
    {
        const Pixmap4 src = img.load4(argv[1],0);
        Engine        eng(par,src);
        const size_t  w = src.w;
        const size_t  h = src.h;

        img.save("img.png",src,0);

        Pixmap3 pxm(w,h);

        //Blur blur(2.2f);
        Blur blur(0.1f);

        blur.applyX<uint8_t,RGB,RGBA,3>(pxm,src,eng);

        img.save("blurx.png",pxm,0);

        std::cerr << "w=" << blur.weight << std::endl;
        std::cerr << "n=" << blur.normal << std::endl;
    }
}
Y_UTEST_DONE()


