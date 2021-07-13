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
    const shared_filters delta  = fdb["Sobel3"];
    color_ramp           cth    = new color_tableau<cold_to_hot>();
    size_to_rgba         sz2c;

    blobs                 Blobs;
    shared_knots          Knots = new knots();
    auto_ptr<edges::blur> Blur = NULL;

    if(argc>1)
    {
        const pixmap<rgba> source = IMG.load<rgba>(argv[1]);

        if(argc>2)
        {
            Blur = new edges::blur(5,5,string_convert::to<float>(argv[2],"sigma"));
        }

        broker             seq(seqEng,source);
        broker             par(seqEng,source);
        edges::detector    E(source.w,source.h);



        IMG.save(source,"source.png");
        std::cerr << "sequential" << std::endl;
        const size_t nseq_full = E.prepare(source,seq,convert<float,rgba>::from,delta, Blur.content());
        const size_t eseq_full = E.extract(Blobs,Knots);
        std::cerr << "nseq_full=" << nseq_full << "=>" << eseq_full << std::endl;
        Blobs.remove_below(3,E);

        E.kmax.save("hist.dat");

        IMG.save(E.grad,"gseq.png",NULL,cth);
        IMG.save(E.kmax,"kseq.png");
        IMG.save(E,"eseq.png",NULL,sz2c);



        std::cerr << "parallel" << std::endl;
        const size_t npar_full = E.prepare(source,par,convert<float,rgba>::from,delta, Blur.content());
        const size_t epar_full = E.extract(Blobs,Knots);
        std::cerr << "npar_full=" << npar_full << "=>" << epar_full << std::endl;
        Blobs.remove_below(3,E);
        IMG.save(E.grad,"gpar.png",NULL,cth);
        IMG.save(E.kmax,"kpar.png");
        IMG.save(E,"epar.png",NULL,sz2c);

        Y_CHECK(npar_full==nseq_full);
        Y_CHECK(epar_full==eseq_full);


    }

}
Y_UTEST_DONE()
