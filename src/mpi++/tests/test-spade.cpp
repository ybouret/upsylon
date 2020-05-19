
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
        MPI.Printf0(stderr, "-------- in %uD --------\n", Dimensions);
        const size_t cores = MPI.size;
        Layout<COORD>       full( Coord::Ones<COORD>(), Coord::Parse<COORD>(layout) );
        vector<COORD>       mappings;
        full.findMappings(mappings,cores);
        for(size_t m=1;m<=mappings.size();++m)
        {
            const COORD &mapping = mappings[m];
            string       usrtext = Coord::ToString(mapping);
            MPI.Printf0(stderr, "using mapping: %s\n", *usrtext);
            const Topology<COORD> topology(mapping);
            const COORD           localRanks = topology.getLocalRanks(MPI.rank);
            typename Layout<COORD>::Loop loop( Coord::Zero<COORD>(), Coord::Ones<COORD>() );

            for(loop.boot();loop.good();loop.next())
            {
                usrtext = Coord::ToString(loop.value);
                MPI.Printf0(stderr, "      |_pbcs : %s\n",*usrtext);
                const Fragment<COORD> fragment(full,localRanks,topology,loop.value,ghosts);
                usrtext = fragment.inner.toString();
                MPI.Printf(stderr," inner: %s\n", *usrtext );
            };

        }

    }

}

Y_UTEST(spade)
{

    Y_MPI(SINGLE);
    string layout = "16:16:16";
    size_t ghosts = 1;

    Coord::DispWidth = 3;
    doTest<Coord1D>(MPI,layout,ghosts);
    doTest<Coord2D>(MPI,layout,ghosts);
    doTest<Coord3D>(MPI,layout,ghosts);





}
Y_UTEST_DONE()

