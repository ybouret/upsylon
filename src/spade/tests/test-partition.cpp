
#include "y/spade/partition.hpp"
#include "y/spade/field3d.hpp"
#include "y/utest/run.hpp"
#include "y/type/spec.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;
using namespace Spade;

namespace {
    
    template <typename COORD>
    static inline void doTest(const string &args,
                              const size_t maxCores)
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
                    Partition<COORD> partition(full,
                                               mapping,
                                               pbcs.value,
                                               1);
                    std::cerr << ": maxItems=" << partition.maxItems;
                    std::cerr << ": maxComms=" << partition.maxComms;
                    std::cerr << std::endl;
                   
                }
                
            }
        }
        
    }
    
}

#include "y/string/convert.hpp"

Y_UTEST(partition)
{
    string args = "3:3:3";
    size_t cores = 4;
    if(argc>1)
    {
        args = argv[1];
    }
    if(argc>2)
    {
        cores = string_convert::to<size_t>(argv[2],"cores");
    }
    doTest<Coord1D>(args,cores);
    doTest<Coord2D>(args,cores);
    doTest<Coord3D>(args,cores);
}
Y_UTEST_DONE()



