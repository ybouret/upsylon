#include "y/gfx/area/tiles.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace GFX;

Y_UTEST(tiles)
{
    
    Area              area(0,0,10,20);
    Topology::Pointer topo = new Topology(area);


    Tiles tiles(2,topo);



}
Y_UTEST_DONE()


