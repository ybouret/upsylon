
#include "y/spade/layouts.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;
using namespace Spade;

namespace {

    template <typename COORD> static inline
    void doTest(const string &layout,
                const string &boundaries,
                const size_t cores)
    {
        static const unsigned                      Dimensions = Coord::Get<COORD>::Dimensions;
        typedef typename Topology<COORD>::Boolean  Boolean;
        const COORD   upper = Coord::Parse<COORD>(layout);
        const COORD   lower = Coord::Ones<COORD>();
        const Boolean pbcs = Coord::ToBool(Coord::Parse<COORD>(boundaries));
        const Layout<COORD> full(lower,upper);
        std::cerr << std::endl;
        std::cerr << "-------- In " << Dimensions << "D --------" << std::endl;
        std::cerr << "full=" << full << std::endl;
        
        vector<COORD> mappings;
        full.findMappings(mappings, cores);


        for(size_t m=1;m<=mappings.size();++m)
        {
            const COORD          &mapping = mappings[m];
            const Topology<COORD> topology(mapping);
            std::cerr << "mapping=" << mapping << ", pbcs=" << pbcs << std::endl;

            for(size_t rank=0;rank<topology.size;++rank)
            {
                const COORD          ranks = topology.getLocalRanks(rank);
                const Layouts<COORD> L(full, ranks, topology, pbcs, 1);
                std::cerr << "\t@localRanks=" << L.ranks << " / globalRank=" << L.rank << std::endl;
                std::cerr << "\t|_inner=" << L.inner << std::endl;
                std::cerr << "\t|_outer=" << L.outer << std::endl;

            }
        }
        
        
    }

}

#include "y/string/convert.hpp"

Y_UTEST(layouts)
{
    string layout     = "10:10:10";
    string boundaries = "0:0:0";
    size_t cores      = 4;

    if(argc>1) layout     = argv[1];
    if(argc>2) boundaries = argv[2];
    if(argc>3) cores      = string_convert::to<size_t>(argv[3],"cores");

    doTest<Coord1D>(layout,boundaries,cores);
    return 0;

    doTest<Coord2D>(layout,boundaries,cores);
    return 0;

    doTest<Coord3D>(layout,boundaries,cores);

}
Y_UTEST_DONE()

