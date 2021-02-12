


#include "y/gfx/pixmap.hpp"
#include "y/gfx/async/broker.hpp"
#include "y/concurrent/scheme/nexus.hpp"

#include "y/utest/run.hpp"
#include "../../main/tests/support.hpp"
#include "y/hashing/sha1.hpp"

#include "y/utest/timings.hpp"
#include <cmath>

#include "y/gfx/async/ops/minmax.hpp"
#include "y/gfx/async/ops/gradient.hpp"


using namespace upsylon;
using namespace GFX;

namespace
{
    template <typename T>
    T id(const T &x)
    {
        return (x);
    }

    template <typename T>
    T sample(const T &x)
    {
        return cos(x);
    }

}

Y_UTEST(broker)
{
    hashing::sha1 H;
    Async::SharedServer seq = new concurrent::sequential_server(); Y_CHECK((**seq).size()==1);
    Async::SharedServer par = new concurrent::nexus();

    {
        unit_t        w = 200;
        unit_t        h = 300;
        Pixmap<float> pf(w,h);
        Async::SharedEngine seqEng = new Async::Engine(pf,(**seq).size());
        Async::SharedEngine parEng = new Async::Engine(pf,(**par).size());

        std::cerr << "seqEng: " << seqEng->size() << " / " << (**seq).size() << std::endl;
        std::cerr << "parEng: " << parEng->size() << " / " << (**par).size() << std::endl;

        Async::Broker seqBrk(seqEng,seq);
        Async::Broker parBrk(parEng,par);

        for(unit_t j=0;j<h;++j)
        {
            for(unit_t i=0;i<w;++i)
            {
                pf[j][i] = support::get<float>();
            }
        }

        const digest md0 = pf.hashWith(H).md();

        Pixmap<float> pfs(pf,seqBrk);
        const digest  mds = pfs.hashWith(H).md();

        Pixmap<float> pfp(pf,parBrk);
        const digest  mdp = pfp.hashWith(H).md();

        std::cerr << "md0=" << md0 << std::endl;
        std::cerr << "mds=" << mds << std::endl;
        std::cerr << "mdp=" << mdp << std::endl;

        Y_CHECK(md0==mds);
        Y_CHECK(md0==mdp);

        pfs.ldz();
        Y_CHECK( pfs.hashWith(H).md() != md0);
        pfs.apply(pf, id<float>, seqBrk);
        const digest md2 = pfs.hashWith(H).md();
        Y_CHECK(md0==md2);

        pfp.ldz();
        Y_CHECK( pfp.hashWith(H).md() != md0);
        pfp.apply(pf, id<float>, parBrk);
        const digest md3 = pfp.hashWith(H).md();
        Y_CHECK(md0==md3);

        std::cerr << "MinMax" << std::endl;
        
        double dMaxSeq = 0;
        double dMaxPar = 0;
        double dMinSeq = 0;
        double dMinPar = 0;

        const Point pmaxSeq = Async::FindMax(dMaxSeq,pf,id<float>,seqBrk);
        const Point pmaxPar = Async::FindMax(dMaxPar,pf,id<float>,parBrk);
        
        Y_CHECK(pmaxSeq==pmaxPar);
        Y_CHECK(fabs(dMaxSeq-dMaxPar)<=0);

        const Point pminSeq = Async::FindMin(dMinSeq,pf,id<float>,seqBrk);
        const Point pminPar = Async::FindMin(dMinPar,pf,id<float>,parBrk);

        Y_CHECK(pminSeq==pminPar);
        Y_CHECK(fabs(dMinSeq-dMinPar)<=0);

        const Point pmax = pmaxSeq;
        const Point pmin = pminSeq;
        Async::MinMax::Info<double> infoSeq, infoPar;
        Async::FindMinMax(infoSeq,pf,id<float>,seqBrk);
        Async::FindMinMax(infoPar,pf,id<float>,parBrk);
        
        Y_CHECK(infoSeq.pmax==pmax);
        Y_CHECK(infoSeq.pmin==pmin);


        
        std::cerr << "Chrono Apply" << std::endl;
        double seq_speed = 0;
        Y_TIMINGS(seq_speed,1, pfs.apply(pf, sample<float>, seqBrk) );
        std::cerr << "seq_speed=" << seq_speed << std::endl;

        double par_speed = 0;
        Y_TIMINGS(par_speed,1, pfp.apply(pf, sample<float>, parBrk) );
        std::cerr << "par_speed=" << par_speed << std::endl;
        
        std::cerr << "efficiency: " <<  parBrk.engine->efficiency(par_speed/seq_speed) << "%" << std::endl;

        
    }



}
Y_UTEST_DONE()
