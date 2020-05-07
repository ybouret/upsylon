#include "y/spade/topology.hpp"
#include "y/utest/run.hpp"
#include "y/core/ipower.hpp"

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
    
    template <typename COORD>
    void doProbes()
    {
        typedef Topology<COORD>     Topo;
        std::cerr << "Probes " << Topo::Dimensions << "D" << std::endl;
        std::cerr << "\tDirections = " << Topo::Directions << std::endl;
        std::cerr << "\tNeighbours = " << Topo::Neighbours << std::endl;
        Y_CHECK(Topo::Neighbours==ipower(3,Topo::Dimensions)-1);
        unsigned i=0;
        for(unsigned j=0;j<Topo::Coordination::Level1;++j,++i)
        {
            Y_ASSERT(i<Topo::Directions);
            const COORD &probe = Topo::Coordination::Probes[i];
            Y_ASSERT(1==Coord::Norm1(probe));
            std::cerr << "\t\t@Level1" << probe << std::endl;
            for(size_t k=0;k<i;++k)
            {
                Y_ASSERT(Topo::Coordination::Probes[k]!=probe);
            }
        }
        std::cerr << std::endl;
        for(unsigned j=0;j<Topo::Coordination::Level2;++j,++i)
        {
            Y_ASSERT(i<Topo::Directions);
            const COORD &probe = Topo::Coordination::Probes[i];
            Y_ASSERT(2==Coord::Norm1(probe));
            std::cerr << "\t\t@Level2" << probe << std::endl;
            for(size_t k=0;k<i;++k)
            {
                Y_ASSERT(Topo::Coordination::Probes[k]!=probe);
            }
        }
        std::cerr << std::endl;
        for(unsigned j=0;j<Topo::Coordination::Level3;++j,++i)
        {
            Y_ASSERT(i<Topo::Directions);
            const COORD &probe = Topo::Coordination::Probes[i];
            Y_ASSERT(3==Coord::Norm1(probe));
            std::cerr << "\t\t@Level3" << probe << std::endl;
            for(size_t k=0;k<i;++k)
            {
                Y_ASSERT(Topo::Coordination::Probes[k]!=probe);
            }
        }
        
        
        
    }
    
}

Y_UTEST(topology)
{
    doTest<Coord1D>();
    doTest<Coord2D>();
    doTest<Coord3D>();
    
    std::cerr << std::endl;
    std::cerr << "Checking prev/next..." << std::endl;
    for(Coord1D size=1;size<=8;++size)
    {
        std::cerr << "size=" << size << std::endl;
        for(Coord1D rank=0;rank<size;++rank)
        {
            const Coord1D prev = Kernel::Topology::Prev(size,rank);
            const Coord1D next = Kernel::Topology::Next(size,rank);
            Y_ASSERT(prev>=0);
            Y_ASSERT(prev<size);
            Y_ASSERT(next>=0);
            Y_ASSERT(next<size);
            Y_ASSERT(rank==Kernel::Topology::Next(size,prev));
            Y_ASSERT(rank==Kernel::Topology::Prev(size,next));

            std::cerr << "\t" << prev << " <- " << rank << " -> " << next << std::endl;
            
        }
    }
    
#if 1
    doProbes<Coord1D>();
    doProbes<Coord2D>();
    doProbes<Coord3D>();
#endif
    
}
Y_UTEST_DONE()


