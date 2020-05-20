
#include "y/spade/network/mpi.hpp"
#include "y/spade/layout/tessellation.hpp"
#include "y/spade/fields.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/sequence/vector.hpp"

using namespace  upsylon;
using namespace  Spade;

namespace {

    template <typename COORD>
    void doTest(mpi          &MPI,
                const string &layout,
                const size_t ghosts)
    {

        static const unsigned Dimensions = Coord::Get<COORD>::Dimensions;
        Y_MPI_HEAD(std::cerr << "In " << Dimensions << "D" << std::endl);

        const size_t cores = MPI.size;
        Layout<COORD>       full( Coord::Ones<COORD>(), Coord::Parse<COORD>(layout) );
        vector<COORD>       mappings;
        full.findMappings(mappings,cores);
        for(size_t m=1;m<=mappings.size();++m)
        {
            const COORD &mapping = mappings[m];
            Y_MPI_HEAD( std::cerr << "using mapping: " << mapping << std::endl );

            const Topology<COORD> topology(mapping);
            const COORD           localRanks = topology.getLocalRanks(MPI.rank);
            typename Layout<COORD>::Loop loop( Coord::Zero<COORD>(), Coord::Ones<COORD>() );

            for(loop.boot();loop.good();loop.next())
            {
                Y_MPI_HEAD( std::cerr << "|_pbcs=" << loop.value << std::endl );
                const Fragment<COORD> fragment(full,localRanks,topology,loop.value,ghosts);
                Y_MPI_NODE(std::cerr << " |_" << MPI.nodeName << std::endl;
                           std::cerr << "  |_inner: " << fragment.inner << std::endl;
                           std::cerr << "  |_outer: " << fragment.outer << std::endl );
                
            };

        }

    }

}

Y_UTEST(spade)
{

    Y_MPI(SINGLE);
    string layout = "16:16:16";
    size_t ghosts = 1;

    Coord::DispWidth = 2;
    doTest<Coord1D>(MPI,layout,ghosts);
    doTest<Coord2D>(MPI,layout,ghosts);
    doTest<Coord3D>(MPI,layout,ghosts);





}
Y_UTEST_DONE()

