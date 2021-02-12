
#include "y/gfx/pixmap.hpp"
#include "y/gfx/async/broker.hpp"
#include "y/concurrent/scheme/nexus.hpp"

#include "y/utest/run.hpp"
#include "../../main/tests/support.hpp"

#include "y/utest/timings.hpp"
#include <cmath>

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
    
    
}

Y_UTEST(grad)
{
    Async::SharedServer seq = new concurrent::sequential_server(); Y_CHECK((**seq).size()==1);
    Async::SharedServer par = new concurrent::nexus();
    
    {
        unit_t        w = 200;
        unit_t        h = 300;
        Pixmap<float> pf(w,h);
        Async::SharedEngine seqEng = new Async::Engine(pf,(**seq).size());
        Async::SharedEngine parEng = new Async::Engine(pf,(**par).size());


        Async::Broker seqBrk(seqEng,seq);
        Async::Broker parBrk(parEng,par);


        for(unit_t j=0;j<h;++j)
        {
            for(unit_t i=0;i<w;++i)
            {
                pf[j][i] = support::get<float>();
            }
        }


        Async::Gradient<double> G(pf.w,pf.h);
        G.compute(pf,seqBrk);
        G.compute(pf,parBrk);

        std::cerr << "Chrono Grad..." << std::endl;
        std::cerr << "seq: " << (*seqBrk).size() << std::endl;
        std::cerr << "par: " << (*parBrk).size() << std::endl;

        double seq_speed = 0;
        Y_TIMINGS(seq_speed,1, G.compute(pf,seqBrk); );
        std::cerr << "seq_speed=" << seq_speed << std::endl;

        double par_speed = 0;
        Y_TIMINGS(par_speed,1, G.compute(pf,parBrk) );
        std::cerr << "par_speed=" << par_speed << std::endl;

        std::cerr << "efficiency: " <<  parBrk.engine->efficiency(par_speed/seq_speed) << "%" << std::endl;
    }
    
}
Y_UTEST_DONE()

