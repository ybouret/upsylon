
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

    Area           area(0,0,w,h);
    Async::Engine  engine(area,2);

    concurrent::nexus srv;

    engine.cycle(srv,Async::Worker::None,NULL);

}
Y_UTEST_DONE()

