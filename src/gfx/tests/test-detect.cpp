#include "y/gfx/edges/detector.hpp"
#include "y/utest/run.hpp"
#include "y/gfx/color/ramp/cold-to-hot.hpp"
#include "y/gfx/color/map.hpp"

#include "y/gfx/image/io.hpp"
#include "y/concurrent/loop/simt.hpp"
#include "y/concurrent/loop/solo.hpp"
#include "y/gfx/color/named.hpp"
#include "y/gfx/filters/db.hpp"
#include "y/gfx/ops/extrema.hpp"

using namespace upsylon;
using namespace graphic;

Y_UTEST(detect)
{
    Y_USE_IMG();
    engine               parEng = new concurrent::simt();
    engine               seqEng = new concurrent::solo();
    filters_db          &fdb    = filters_db::instance();
    const shared_filters delta  = fdb["Sobel7"];
    color_ramp           cth    = new color_tableau<cold_to_hot>();

    if(argc>1)
    {
        const pixmap<rgba> source = IMG.load<rgba>(argv[1]);
        broker             seq(seqEng,source);
        broker             par(seqEng,source);
        edges::detector    E(source.w,source.h);

        IMG.save(source,"source.png");
        std::cerr << "sequential/no-blur" << std::endl;
        E.build(source,seq,convert<float,rgba>::from,delta, NULL);
        IMG.save(E.grad,"gseq-full.png",NULL,cth);
        IMG.save(E.kmax,"kseq-full.png");

        std::cerr << "parallel/no-blur" << std::endl;
        E.build(source,par,convert<float,rgba>::from,delta, NULL);
        IMG.save(E.grad,"gpar-full.png",NULL,cth);
        IMG.save(E.kmax,"kpar-full.png");

    }

}
Y_UTEST_DONE()
