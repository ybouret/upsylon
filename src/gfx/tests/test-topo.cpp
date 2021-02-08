

#include "y/gfx/topology.hpp"
#include "y/utest/run.hpp"
#include <cstdlib>

using namespace upsylon;
using namespace GFX;

Y_UTEST(topo)
{
    unit_t w = 10;
    unit_t h = 20;

    if(argc>1) w = atol(argv[1]);
    if(argc>2) h = atol(argv[2]);


    Area     area(0,0,w,h);      std::cerr << "area   = " << area << std::endl;
    Topology topo(area);

    std::cerr << topo << std::endl;

}
Y_UTEST_DONE()

