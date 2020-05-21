
#include "y/spade/network/mpi.hpp"
#include "y/spade/workspace.hpp"
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
        //typedef typename FieldFor<COORD>::template Of<int>::Type    iField;
        //typedef typename FieldFor<COORD>::template Of<string>::Type sField;
        //typedef typename FieldFor<COORD>::template Of<double>::Type dField;

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

            typename Layout<COORD>::Loop loop( Coord::Zero<COORD>(), Coord::Ones<COORD>() );

            Synchronize sync(MPI,comms::distributed);
            sync.query<string>();

            for(loop.boot();loop.good();loop.next())
            {
                // try every periodic boundary condition
                Y_MPI_HEAD( std::cerr << "|_pbcs=" << loop.value << std::endl );
                Workspace<COORD> W( full, mapping, MPI.rank, loop.value, ghosts);
                Y_MPI_NODE(std::cerr << " |_" << MPI.nodeName << std::endl;
                           std::cerr << "  |_inner: " << W.inner << std::endl;
                           std::cerr << "  |_outer: " << W.outer << std::endl );

                W.template create<int>(    "I" );
                W.template create<double>( "D" );
                W.template create<double>( "Dtmp", LocalField );
                W.template create<string>( "S" );
                W.activateFor(sync);
                
                //sync.asyncStyle(W.fields);
                W.localSwap(sync);
                IOBlock send, recv;
                sync.forward( aliasing::_(W.fields), W, send, recv);
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

