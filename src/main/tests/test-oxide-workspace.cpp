#include "y/oxide/workspace.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;
using namespace Oxide;

namespace
{
    template <typename COORD>
    static inline void do_wksp (const Layout<COORD> &full,
                                const size_t         cores,
                                const COORD          pbc,
                                const size_t         ng)
    {
        std::cerr << "---------------- In " << Coord::Get<COORD>::Dimensions << "D ----------------" << std::endl;

        std::cerr << "full =" << full << std::endl;
        std::cerr << "cores=" << cores << std::endl;
        vector<COORD>  mappings;
        full.buildMappings(mappings,cores);
        std::cerr << "\tmappings: " << mappings << std::endl;
        for(size_t m=1;m<=mappings.size();++m)
        {
            const COORD &sizes = mappings[m]; Y_ASSERT( cores == size_t(Coord::Product(sizes)) );
            std::cerr << "\t\tusing " << sizes << std::endl;
            for(size_t rank = 0; rank<cores; ++rank )
            {
                const Workspace<COORD> W(full,sizes,rank,pbc,ng);
                std::cerr << "\t\ttile[" << rank << "]=" << W.inner << "->" << W.outer << std::endl;
            }
            std::cerr << std::endl;
        }
        std::cerr << std::endl;
    }
}

#include "y/string/convert.hpp"

Y_UTEST(oxide_workspace)
{
    size_t cores=2; if(argc>1) cores = string_convert::to<size_t>(argv[1],"cores");

    const Coord3D org(1,1,1);
    Coord3D       top(10,20,30);
    if(argc>2)    top = Coord::Parse<Coord3D>(argv[2],"top");

    Coord3D       pbc(0,0,0);
    if(argc>3)    pbc = Coord::Parse<Coord3D>(argv[3],"pbc");

    size_t ng = 1;
    if(argc>4)   ng = string_convert::to<size_t>(argv[4],"ng");


    const Layout1D L1( org.x,    top.x    );
    const Layout2D L2( org.xy(), top.xy() );
    const Layout3D L3( org,      top );

    
    do_wksp<Coord1D>(L1,cores,pbc.x,ng);
    do_wksp<Coord2D>(L2,cores,pbc.xy(),ng);
    do_wksp<Coord3D>(L3,cores,pbc,ng);

    
}
Y_UTEST_DONE()
