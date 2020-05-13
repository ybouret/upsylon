
#include "y/spade/layouts.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;
using namespace Spade;

namespace {

    template <typename COORD> static inline
    void doTest(const string &layout, const size_t cores)
    {
        const COORD upper = Coord::Parse<COORD>(layout);
        const COORD lower = Coord::Ones<COORD>();
        const Layout<COORD> full(lower,upper);
        std::cerr << "full=" << full << std::endl;
        
        vector<COORD> mappings;
        full.findMappings(mappings, cores);
        
        for(size_t m=1;m<=mappings.size();++m)
        {
            const COORD          &mapping = mappings[m];
            std::cerr << "using " << mapping << std::endl;
            const Topology<COORD> topology(mapping);
            for(size_t rank=0;rank<topology.size;++rank)
            {
            }
        }
        
        
    }

}

Y_UTEST(layouts)
{
    string layout = "10:10:10";
    size_t cores  = 4;
    
    doTest<Coord1D>(layout,cores);
    doTest<Coord2D>(layout,cores);
    
    return 0;
    doTest<Coord3D>(layout,cores);

}
Y_UTEST_DONE()

