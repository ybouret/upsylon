

#include "y/gfx/area/topology.hpp"
#include "y/utest/run.hpp"
#include <cstdlib>

using namespace upsylon;
using namespace GFX;

Y_UTEST(topo)
{


    for(unit_t w=1;w<=10;++w)
    {
        for(unit_t h=1;h<=10;++h)
        {
            Area     area(alea.range<unit_t>(-100,100),
                          alea.range<unit_t>(-100,100),
                          w,h);
            std::cerr << "area   = " << area << std::endl;
            Topology topo(area);

            std::cerr << topo << std::endl;
            Y_CHECK(topo.n==topo.inner+topo.outer);
            std::cerr << std::endl;
        }

    }
}
Y_UTEST_DONE()

