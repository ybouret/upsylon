#include "y/spade/layout/dispatch.hpp"
#include "y/utest/run.hpp"
#include "y/core/ipower.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace Spade;

namespace {
 
    template <typename COORD>
    void doTest()
    {
        typedef Dispatch<COORD>        Topo;
        typedef typename Topo::Node    Node;
        typedef typename Topo::Loop    Loop;

        std::cerr << std::endl;
        std::cerr << "In " << Topo::Dimensions << "D" << std::endl;
        for(unsigned dim = 0; dim<Topo::Dimensions; ++dim )
        {
            Y_ASSERT(Coord::IsUnitVector(dim,Topo::Coordination::Probes[dim]));
        }
        
        const COORD smax = Coord::Ld<COORD>(4);
        const COORD smin = Coord::Ones<COORD>();
        Loop        loop(smin,smax);
        Loop        lpbc(Coord::Zero<COORD>(),Coord::Ones<COORD>());

        const COORD &sizes = loop.value;
        for(loop.boot();loop.good();loop.next())
        {
            Topo topo(sizes);
            std::cerr << sizes << " -> #cores=" << topo.size << std::endl;
            std::cerr << "parallel=" << topo.parallel << std::endl;
            std::cerr << "checking ranks..." << std::endl;
            for(size_t rank=0;rank<topo.size;++rank)
            {
                const COORD  ranks = topo.getLocalRanks(rank);
                const size_t gRank = topo.getGlobalRank(ranks);
                Y_ASSERT(gRank==rank);
            }
            std::cerr << "checking connectivity..." << std::endl;
            
            const COORD &pbc = lpbc.value;
            for(lpbc.boot();lpbc.good();lpbc.next())
            {
                for(size_t rank=0;rank<topo.size;++rank)
                {
                    const COORD  ranks = topo.getLocalRanks(rank);
                    std::cerr << "ranks=" << ranks << " / pbc=" << pbc <<  std::endl;
                    const Node   node(ranks,topo,pbc);
                }
            }
        }
    }
    
    template <typename COORD>
    void doProbes()
    {
        typedef Dispatch<COORD>     Topo;
        std::cerr << "Probes " << Topo::Dimensions << "D" << std::endl;
        std::cerr << "\tLevels = " << Topo::Levels << std::endl;
        Y_CHECK(Topo::Levels*2==ipower<unsigned>(3,Topo::Dimensions)-1);
        unsigned i=0;
        for(unsigned j=0;j<Topo::Coordination::Level1;++j,++i)
        {
            Y_ASSERT(i<Topo::Levels);
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
            Y_ASSERT(i<Topo::Levels);
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
            Y_ASSERT(i<Topo::Levels);
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


#include "y/ios/ocstream.hpp"

Y_UTEST(dispatch)
{
    Coord::DispWidth = 2;
    std::cerr << std::endl;
    std::cerr << "Checking prev/next..." << std::endl;
    for(Coord1D size=1;size<=8;++size)
    {
        std::cerr << "size=" << size << std::endl;
        for(Coord1D rank=0;rank<size;++rank)
        {
            const Coord1D prev = Kernel::Dispatch::Prev(size,rank);
            const Coord1D next = Kernel::Dispatch::Next(size,rank);
            Y_ASSERT(prev>=0);
            Y_ASSERT(prev<size);
            Y_ASSERT(next>=0);
            Y_ASSERT(next<size);
            Y_ASSERT(rank==Kernel::Dispatch::Next(size,prev));
            Y_ASSERT(rank==Kernel::Dispatch::Prev(size,next));
            
            std::cerr << "\t" << prev << " <- " << rank << " -> " << next << std::endl;
            
        }
    }
    
    doProbes<Coord1D>();
    doProbes<Coord2D>();
    doProbes<Coord3D>();

    doTest<Coord1D>();
    doTest<Coord2D>();
    doTest<Coord3D>();
    
    Y_UTEST_SIZEOF(Dispatch<Coord1D>::Links);
    Y_UTEST_SIZEOF(Dispatch<Coord2D>::Links);
    Y_UTEST_SIZEOF(Dispatch<Coord3D>::Links);

    Y_UTEST_SIZEOF(Dispatch<Coord1D>::Node);
    Y_UTEST_SIZEOF(Dispatch<Coord2D>::Node);
    Y_UTEST_SIZEOF(Dispatch<Coord3D>::Node);

#define SHOW(VALUE) std::cerr << #VALUE << " = " << (VALUE) << std::endl

    SHOW( Connect::FreeStanding );
    SHOW( Connect::AsyncReverse );
    SHOW( Connect::AutoExchange );
    SHOW( Connect::AsyncForward );
    SHOW( Connect::AsyncTwoWays );


    {
        ios::ocstream fp("topo.dat");
        int y=0;
        fp("%d %d\n",Connect::AutoExchange,y++);
        fp("%d %d\n",Connect::AsyncTwoWays,y++);
        fp("%d %d\n",Connect::AsyncForward,y++);
        fp("%d %d\n",Connect::AsyncReverse,y++);
    }
    
#define SHOW_CLASS(VALUE) std::cerr << "CLASS_OF(" << #VALUE << ")=" << Y_SPADE_CLASS_OF(VALUE) << std::endl

    SHOW_CLASS(Connect::AutoExchange);
    SHOW_CLASS(Connect::AsyncTwoWays);
    SHOW_CLASS(Connect::AsyncForward);
    SHOW_CLASS(Connect::AsyncReverse);
    
}
Y_UTEST_DONE()


