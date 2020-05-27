
#include "y/spade/layout/tessellation.hpp"
#include "y/spade/fields.hpp"
#include "y/utest/run.hpp"
#include "y/type/spec.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;
using namespace Spade;

namespace {
    
    template <typename COORD>
    static inline void doTest(const string &args,
                              const size_t maxCores,
                              const size_t numGhosts)
    {
        std::cerr << std::endl;
        std::cerr << "Layout<" << type_name_of<COORD>() << ">" << std::endl;
        const COORD   width = Coord::Parse<COORD>(args);
        const COORD   shift = Coord::Integer( 10 * Coord::Ones<COORD>(), alea);
        const COORD   lo    = Coord::Ones<COORD>() + shift;
        const COORD   up    = width + shift;
        Coord::Disp(std::cerr << "lo=",lo) << std::endl;
        Coord::Disp(std::cerr << "up=",up) << std::endl;
      
       
        Layout<COORD>    full(lo,up);
        Y_CHECK(width == full.width);
        vector<COORD>     mv;
        list<const COORD> ml;
        for(size_t cores=1;cores<=maxCores;++cores)
        {
            std::cerr << "\t#cores=" << cores;
            full.findMappings(mv,cores);
            full.findMappings(ml,cores);
            Y_ASSERT(mv.size()==ml.size());
            std::cerr << " : " << mv << std::endl;
            for(size_t i=1;i<=mv.size();++i)
            {
                const COORD &mapping = mv[i];
                std::cerr << "\t\t" << mapping << std::endl;
                typename Layout<COORD>::Loop pbcs( Coord::Zero<COORD>(), Coord::Ones<COORD>() );
                for( pbcs.boot(); pbcs.good(); pbcs.next() )
                {
                    std::cerr << "\t\t\tpbcs=" << pbcs.value;
                    Tessellation<COORD> tess(full,
                                             mapping,
                                             pbcs.value,
                                             numGhosts);
                    std::cerr << ": maxItems=" << tess.maxItems;
                    std::cerr << ": maxComms=" << tess.maxComms;
                    std::cerr << std::endl;

                }
            }
        }



        
    }

    template <typename COORD>
    static inline void doFind(const string &args,
                              const size_t maxCores,
                              const size_t numGhosts)
    {
        std::cerr << std::endl;
        std::cerr << "Layout<" << type_name_of<COORD>() << ">" << std::endl;
        const COORD   width = Coord::Parse<COORD>(args);
        const COORD   shift = Coord::Integer( 10 * Coord::Ones<COORD>(), alea);
        const COORD   lo    = Coord::Ones<COORD>() + shift;
        const COORD   up    = width + shift;
        Layout<COORD> full(lo,up);

        for(size_t cores=1;cores<=maxCores;++cores)
        {
            std::cerr << "\tcores=" << cores << std::endl;
            typename Layout<COORD>::Loop pbcs( Coord::Zero<COORD>(), Coord::Ones<COORD>() );
            for( pbcs.boot(); pbcs.good(); pbcs.next() )
            {
                std::cerr << "\t\t\tpbcs=" << pbcs.value << std::endl;
                const COORD opt = Tessellation<COORD>::Find(full, cores, pbcs.value, numGhosts);
            }
        }
    }
    
}

#include "y/string/convert.hpp"

Y_UTEST(tessellation)
{
    string args   = "8:8:8";
    size_t cores  = 4;
    size_t ghosts = 1;
    if(argc>1)
    {
        args = argv[1];
    }
    if(argc>2)
    {
        cores = string_convert::to<size_t>(argv[2],"cores");
    }
    if(argc>3)
    {
        ghosts = string_convert::to<size_t>(argv[3],"ghosts");
    }

    doTest<Coord1D>(args,cores,ghosts);
    doTest<Coord2D>(args,cores,ghosts);
    doTest<Coord3D>(args,cores,ghosts);

    doFind<Coord1D>(args,cores,ghosts);
    doFind<Coord2D>(args,cores,ghosts);
    doFind<Coord3D>(args,cores,ghosts);

}
Y_UTEST_DONE()



