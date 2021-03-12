
#include "y/gfx/ops/hist.hpp"
#include "y/concurrent/loop/simt.hpp"
#include "y/concurrent/loop/solo.hpp"
#include "y/gfx/pixmaps.hpp"

#include "y/utest/run.hpp"
#include "y/utest/timings.hpp"

#include "y/gfx/image/io.hpp"

using namespace upsylon;
using namespace graphic;


namespace {

}

Y_UTEST(ops)
{
    image::io &IMG = Y_IMAGE();
    engine seqEngine = new concurrent::solo();
    engine parEngine = new concurrent::simt();

    if(argc>1)
    {
        const pixmap<rgb> img = IMG.load<rgb>(argv[1]);
        broker            seq(seqEngine,img);
        broker            par(parEngine,img);
        



        {
            std::cerr << "computing histograms..." << std::endl;
            histogram         Hseq;
            Hseq.reset();
            compute_histogram::update(Hseq,img,seq, convert<uint8_t,rgb>::from );

            histogram Hpar;
            Hpar.reset();
            compute_histogram::update(Hpar,img,par, convert<uint8_t,rgb>::from );

            Hseq.save("hseq.dat");
            Hpar.save("hpar.dat");

            double seq_speed = 0;
            Y_TIMINGS(seq_speed,1,compute_histogram::update(Hseq,img,seq, convert<uint8_t,rgb>::from ) );
            std::cerr << "seq_speed=" << seq_speed << std::endl;

            double par_speed = 0;
            Y_TIMINGS(par_speed,1,compute_histogram::update(Hpar,img,par,convert<uint8_t,rgb>::from ) );
            std::cerr << "par_speed=" << par_speed << std::endl;
            std::cerr << "efficiency: " << parEngine->efficiency(par_speed/seq_speed) << std::endl;
        }
    }
}
Y_UTEST_DONE()
