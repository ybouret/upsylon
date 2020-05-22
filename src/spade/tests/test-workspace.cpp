

#include "y/spade/workspace.hpp"
#include "y/spade/layout/tessellation.hpp"
#include "y/spade/network/transfer.hpp"

#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/string/convert.hpp"
#include "y/spade/field/ops.hpp"
#include "y/ios/imstream.hpp"

using namespace upsylon;
using namespace Spade;

namespace {

    template <typename COORD> static inline
    void doTest( const string &layout, const size_t ghosts, const size_t cores )
    {
        typedef typename FieldFor<COORD>::template Of<int>::Type    iField;
        typedef typename FieldFor<COORD>::template Of<string>::Type sField;
        typedef typename FieldFor<COORD>::template Of<double>::Type dField;

        Transfer transfer(comms::homogeneous);
        (void) transfer.query<string>();


        const COORD   lower = Coord::Ones<COORD>();
        const COORD   upper = Coord::Parse<COORD>(layout);
        const Coord1D ng    = Coord1D(ghosts);
        const Layout<COORD> fullLayout(lower,upper);
        vector<COORD>       mappings;
        fullLayout.findMappings(mappings,cores);

        std::cerr << "mappings=" << mappings << std::endl;
        for(size_t i=1;i<=mappings.size();++i)
        {
            const COORD          &mapping = mappings[i];

            std::cerr << "using " << mapping << std::endl;
            typename Layout<COORD>::Loop loop( Coord::Zero<COORD>(), Coord::Ones<COORD>() );
            for(loop.boot();loop.good();loop.next())
            {
                std::cerr << "\tpbcs=" << loop.value << std::endl;

                for(size_t rank=0;rank<cores;++rank)
                {
                    Workspace<COORD> W( fullLayout, mapping, rank, loop.value, ng);
                    Y_ASSERT(W.rank==rank);
                    Y_ASSERT(W.size==cores);
                    
                    iField &I = W.template create<int>( "I" );
                    sField &S = W.template create<string>( "S" );
                    dField &D = W.template create<double>( "D" );

                    Y_ASSERT(I.items==W.outer.items);
                    Y_ASSERT(S.items==W.outer.items);
                    Y_ASSERT(D.items==W.outer.items);
                    Y_ASSERT( &I == & W["I"] );
                    
                    
                    
                    W.setupWith(transfer);
                    W.localSwap(transfer);
                }

            }
        }

    }

}

Y_UTEST(workspace)
{
    string layout     = "10:10:10";
    size_t ghosts     = 1;
    size_t cores      = 2;

    if(argc>1) layout = argv[1];
    if(argc>2) ghosts = string_convert::to<size_t>(argv[2],"ghosts");
    if(argc>3) cores  = string_convert::to<size_t>(argv[3],"cores");

    doTest<Coord1D>(layout,ghosts,cores);
    doTest<Coord2D>(layout,ghosts,cores);
    doTest<Coord3D>(layout,ghosts,cores);
}
Y_UTEST_DONE()

