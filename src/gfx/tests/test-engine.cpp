
#include "y/gfx/async/engine.hpp"
#include "y/concurrent/scheme/nexus.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace GFX;


Y_UTEST(engine)
{
    unit_t w = 10;
    unit_t h = 20;

    if(argc>1)
    {
        const Point p = Parse::WxH(argv[1]);
        w=p.x;
        h=p.y;
    }

    Area                          area(0,0,w,h);
    Topology::Pointer             topo = new Topology(area);
    Async::Engine                 engine(4,topo);
    concurrent::sequential_server seq;
    concurrent::nexus             par;

    engine.cache<float>(1);

    engine.cycle(seq);
    engine.cycle(par);

}
Y_UTEST_DONE()

