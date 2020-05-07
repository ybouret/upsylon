#include "y/spade/topology.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Spade;

namespace {
 
    template <typename COORD>
    void doTest()
    {
        typedef Topology<COORD>     Topo;
        typedef typename Topo::Loop Loop;
        
        std::cerr << std::endl;
        std::cerr << "In " << Topo::Dimensions << "D" << std::endl;

        
        const COORD smax = Coord::Ld<COORD>(4);
        const COORD smin = Coord::Ones<COORD>();
        Loop        loop(smin,smax);
        
        const COORD &sizes = loop.value;
        for(loop.boot();loop.good();loop.next())
        {
            Topo topo(sizes);
            std::cerr << sizes << " -> #cores=" << topo.size << std::endl;
            for(size_t rank=0;rank<topo.size;++rank)
            {
                const COORD  ranks = topo.getLocalRanks(rank);
                const size_t gRank = topo.getGlobalRank(ranks);
                
                std::cerr << "\t" << ranks << " -> " << gRank << std::endl;
                Y_ASSERT(gRank==rank);
            }
        }
        
    }
}

Y_UTEST(topology)
{
    doTest<Coord1D>();
    doTest<Coord2D>();
    doTest<Coord3D>();

}
Y_UTEST_DONE()


