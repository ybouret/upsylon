#include "y/ink/ops/blur.hpp"
#include "y/utest/run.hpp"
#include "y/ink/image.hpp"
#include "y/string/convert.hpp"
#include "y/ink/ops/filter.hpp"

using namespace upsylon;
using namespace Ink;

Y_UTEST(blur)
{
    for(float sig=0.1f;sig<=5.0f;sig+=0.1f)
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
        float         r   = 1.2f;
        if(argc>2)
        {
            r = string_convert::to<float>(argv[2],"r");
        }
        Engine        eng(par,src);
        const size_t  w = src.w;
        const size_t  h = src.h;
        
        img.save("img.png",src,0);


        Pixmap3 pxm(w,h);
        Blur    blur(r);

        blur._applyX<uint8_t,RGB,RGBA,3>(pxm,src,Crux::ClosestByte,eng);

        img.save("blurx.png",pxm,0);
        blur._applyY<uint8_t,RGB,RGBA,3>(pxm,src,Crux::ClosestByte,eng);
        img.save("blury.png",pxm,0);

        Pixmap3 tmp(w,h);

        blur._apply<uint8_t,RGB,RGBA,3>(pxm,tmp,src,Crux::ClosestByte,eng);
        img.save("blur.png",pxm,0);

        PixmapF pxmf(w,h);
        Filter::Apply(pxmf,src,RGBAtoFloat,eng);
        img.save("imgf.png",pxmf,0);

        PixmapF tmpf(w,h);
        PixmapF tgtf(w,h);
        blur.apply(tgtf,tmpf,pxmf,eng);

        img.save("blurf.png",tgtf,0);

        std::cerr << "blur.w=" << blur.weight << std::endl;
        std::cerr << "blur.n=" << blur.normal << std::endl;
    }
}
Y_UTEST_DONE()


