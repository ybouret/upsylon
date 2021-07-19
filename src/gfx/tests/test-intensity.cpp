#include "y/gfx/ops/intensity.hpp"
#include "y/utest/run.hpp"
#include "y/gfx/image/io.hpp"
#include "y/gfx/image/io.hpp"
#include "y/concurrent/loop/simt.hpp"
#include "y/concurrent/loop/solo.hpp"

using namespace upsylon;
using namespace graphic;

Y_UTEST(intensity)
{
    image::io &IMG   = Y_IMAGE();
    engine seqEngine = new concurrent::solo();
    engine parEngine = new concurrent::simt();


    if(argc>1)
    {
        const pixmap<rgb> img = IMG.load<rgb>(argv[1]);
        IMG.save(img,"img.png");
        broker            seq(seqEngine,img);
        broker            par(parEngine,img);

        intensity Iseq,Ipar;
        Iseq.scan(img,seq);std::cerr << "seq: vmin=" << Iseq.vmin << " | vmax=" << Iseq.vmax << std::endl;
        Ipar.scan(img,par);std::cerr << "par: vmin=" << Ipar.vmin << " | vmax=" << Ipar.vmax << std::endl;

        

    }

}
Y_UTEST_DONE()
