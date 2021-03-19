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
        engine             seqEngine = new concurrent::solo();
        broker             seq(seqEngine,img);

        engine             parEngine = new concurrent::simt();
        broker             par(parEngine,img);

        IMG.save(img,"img.png");

        pixmap<rgb> pxm(img.w,img.h);

        std::cerr << "blur RGB seq..." << std::endl;
        b.compute(pxm,seq,img);
        IMG.save(pxm,"blur3-seq.png");

        std::cerr << "blur RGB par..." << std::endl;
        b.compute(pxm,par,img);
        IMG.save(pxm,"blur3-par.png");

        const pixmap<float> imgf(img,seq,convert<float,rgb>::from);
        pixmap<float>       pxmf(img.w,img.h);
        std::cerr << "blur float seq..." << std::endl;
        b.compute(pxmf,seq,imgf);
        IMG.save(pxmf,"blurf-seq.png");

        std::cerr << "blur float par..." << std::endl;
        b.compute(pxmf,par,imgf);
        IMG.save(pxmf,"blurf-par.png");
        

    }



}
Y_UTEST_DONE()

