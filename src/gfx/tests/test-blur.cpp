#include "y/gfx/ops/blur.hpp"
#include "y/utest/run.hpp"
#include "y/gfx/image/io.hpp"

#include "y/concurrent/loop/simt.hpp"
#include "y/concurrent/loop/solo.hpp"

using namespace upsylon;
using namespace graphic;

namespace
{


}

Y_UTEST(blur)
{
    Y_USE_IMG();

    float sigma = 2.7f;

    if(argc>1)
    {
        sigma = string_convert::to<float>(argv[1],"sigma");
    }
    blur  b(sigma);
    std::cerr << "sigma=" << sigma << std::endl;

    if(argc>2)
    {
        const pixmap<rgb>  img = IMG.load<rgb>(argv[2]);
        engine             seqEngine = new concurrent::solo;
        broker             seq(seqEngine,img);
        const tile        &full = seq[0];

        IMG.save(img,"img.png");

        full.cache->make<rgb>( b.data.size );
        std::cerr << "full blur RGB" << std::endl;

        pixmap<rgb> pxm(img.w,img.h);

        
        b.compute<rgb,uint8_t,3>(full,pxm,img);

        IMG.save(pxm,"blur3.png");

        const pixmap<float> imgf(img,seq,convert<float,rgb>::from);
        pixmap<float>       pxmf(img.w,img.h);
        std::cerr << "full blur float" << std::endl;

        full.cache->make<float>( b.data.size );

        b.compute<float,float,1>(full,pxmf,imgf);

        IMG.save(imgf,"imgf.png");
        IMG.save(pxmf,"blurf.png");

    }



}
Y_UTEST_DONE()

