
#include "y/spade/network/mpi.hpp"
#include "y/spade/workspace.hpp"
#include "y/spade/fields.hpp"
#include "y/spade/field/ops.hpp"
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
        typedef typename FieldFor<COORD>::template Of<int>::Type    iField;
        typedef typename FieldFor<COORD>::template Of<string>::Type sField;
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
                //Workspace<COORD> W( full, mapping, MPI.rank, loop.value, ghosts);
                Domain<COORD>    W(MPI,full,mapping,loop.value,ghosts);
                Y_MPI_NODE(std::cerr << " |_" << MPI.nodeName << std::endl;
                           std::cerr << "  |_inner: " << W.inner << std::endl;
                           std::cerr << "  |_outer: " << W.outer << std::endl );

                iField &I = W.template create<int>(    "I" );
                iField &J = W.template create<int>(    "J" );
                (void)      W.template create<double>( "D" );
                (void)      W.template create<double>( "Dtmp", LocalField );
                sField &S = W.template create<string>( "S" );
                
                FieldsIO  all = W.fields;
                FieldsIO  sub;
                sub << I << W["D"];
                
                W.setupWith(sync);
                W.localSwap(all,sync);
                W.localSwap(sub,sync);
                

                sync.asyncSetup(all); Y_ASSERT(sync.style==comms::flexible_block_size);
                W.exchange(all,sync);


                sync.asyncSetup(I); Y_ASSERT(sync.style==comms::computed_block_size);
                W.exchange(I,sync);


                sync.asyncSetup(S); Y_ASSERT(sync.style==comms::flexible_block_size);
                W.exchange(S,sync);


                sync.asyncSetup(sub);

                W.exchange(sub,sync);

                // check exchanges are OK!
                sub.free();
                sub << I << J;
                sync.asyncSetup(sub);
                Y_ASSERT(sync.style==comms::computed_block_size);
                Y_ASSERT(sync.chunk==sizeof(int)+sizeof(int));

                Ops::Ld(I,I,-1);
                Ops::Ld(J,J, 2);
                Ops::Ld(I,W.inner,MPI.rank);
                Ops::Ld(J,W.inner,-MPI.rank);


                W.exchange(sub,sync);


                for(size_t i=0;i<W.numAutoExchange;++i)
                {
                    Y_ASSERT( !Ops::CountDiff(I,W.autoExchange[i].forward->outerRange,MPI.rank) );
                    Y_ASSERT( !Ops::CountDiff(I,W.autoExchange[i].reverse->outerRange,MPI.rank) );
                    Y_ASSERT( !Ops::CountDiff(J,W.autoExchange[i].forward->outerRange,-MPI.rank) );
                    Y_ASSERT( !Ops::CountDiff(J,W.autoExchange[i].reverse->outerRange,-MPI.rank) );
                }

                for(size_t i=0;i<W.numAsyncTwoWays;++i)
                {
                    Y_ASSERT( !Ops::CountDiff(I,W.asyncTwoWays[i].forward->outerRange, int(W.asyncTwoWays[i].forward->peer)) );
                    Y_ASSERT( !Ops::CountDiff(I,W.asyncTwoWays[i].reverse->outerRange, int(W.asyncTwoWays[i].reverse->peer)) );
                    Y_ASSERT( !Ops::CountDiff(J,W.asyncTwoWays[i].forward->outerRange,-int(W.asyncTwoWays[i].forward->peer) ) );
                    Y_ASSERT( !Ops::CountDiff(J,W.asyncTwoWays[i].reverse->outerRange,-int(W.asyncTwoWays[i].reverse->peer) ) );
                }

                for(size_t i=0;i<W.numAsyncForward;++i)
                {
                    Y_ASSERT( !Ops::CountDiff(I,W.asyncForward[i].forward->outerRange, int(W.asyncForward[i].forward->peer)) );
                    Y_ASSERT( !Ops::CountDiff(J,W.asyncForward[i].forward->outerRange,-int(W.asyncForward[i].forward->peer)) );
                }

                for(size_t i=0;i<W.numAsyncReverse;++i)
                {
                    Y_ASSERT( !Ops::CountDiff(I,W.asyncReverse[i].reverse->outerRange, int(W.asyncReverse[i].reverse->peer)) );
                    Y_ASSERT( !Ops::CountDiff(J,W.asyncReverse[i].reverse->outerRange,-int(W.asyncReverse[i].reverse->peer)) );
                }

            };
            Y_MPI_NODE(std::cerr << MPI.nodeName << " send: " << MPI.commSend.data.full << " | recv: " << MPI.commRecv.data.full << std::endl);
        }

    }

}

#include "y/string/convert.hpp"

Y_UTEST(spade)
{

    Y_MPI(SINGLE);
    string layout = "16:16:16";
    size_t ghosts = 1;
    if(argc>1) layout = argv[1];
    if(argc>2) ghosts = string_convert::to<size_t>(argv[2],"ghosts");
    
    Coord::DispWidth = 2;
    doTest<Coord1D>(MPI,layout,ghosts);
    doTest<Coord2D>(MPI,layout,ghosts);
    doTest<Coord3D>(MPI,layout,ghosts);





}
Y_UTEST_DONE()

