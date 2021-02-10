


#include "y/gfx/pixmap.hpp"
#include "y/gfx/async/broker.hpp"
#include "y/concurrent/scheme/nexus.hpp"

#include "y/utest/run.hpp"
#include "../../main/tests/support.hpp"
#include "y/hashing/sha1.hpp"

#include "y/utest/timings.hpp"

using namespace upsylon;
using namespace GFX;


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
        
    }



}
Y_UTEST_DONE()
