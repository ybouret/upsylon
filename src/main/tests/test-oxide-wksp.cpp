#include "y/oxide/workspace.hpp"
#include "y/utest/run.hpp"
#include "y/memory/pooled.hpp"
#include "y/string/tokenizer.hpp"

using namespace upsylon;
using namespace Oxide;

namespace
{
    typedef vector<string,memory::pooled> strings;
    static inline bool isSep( const char C ) { return C == ','; }

    template <typename COORD>
    static inline void testWksp(char **argv)
    {
        const COORD   length  = Coord::Parse<COORD>(argv[1],"length");
        const COORD   pbc     = Coord::Parse<COORD>(argv[2],"pbc");
        const Coord1D ng      = Coord::Parse<Coord1D>(argv[3],"ng");
        const COORD   mapping = Coord::Parse<COORD>(argv[4],"mapping");

        COORD org(0); Coord::LD(org,1);
        const Layout<COORD> full(org,length);
        std::cerr << "full=" << full << std::endl;

        const Coord1D size = Coord::Product(mapping);
        for(Coord1D   rank=0;rank<size;++rank)
        {
            std::cerr << "tile[" << rank << "]" << std::endl;

            Workspace<COORD> wksp(full,mapping,rank,pbc,ng);
            wksp.display(std::cerr, "\t(*) ");
        }


    }

}

Y_UTEST(oxide_wksp)
{
    // length pbc ng mapping
    if(argc<=4)
    {
        std::cerr << "Usage: " << program << " length pbc ng mapping" << std::endl;
        return 0;
    }


    strings      length(3,as_capacity);
    const size_t dimensions = tokenizer<char>::split(length, argv[1], isSep);


    switch(dimensions)
    {
        case 1: testWksp<Coord1D>(argv); break;
        case 2: testWksp<Coord2D>(argv); break;
        case 3: testWksp<Coord3D>(argv); break;
        default: break;
    }

}
Y_UTEST_DONE()
