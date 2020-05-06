
#include "y/spade/partition.hpp"
#include "y/utest/run.hpp"
#include "y/type/spec.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;
using namespace Spade;

namespace {
    
    template <typename COORD>
    static inline void doTest(const string &args)
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
        for(size_t cores=1;cores<=8;++cores)
        {
            std::cerr << "\t#cores=" << cores;
            full.findMappings(mv,cores);
            full.findMappings(ml,cores);
            Y_ASSERT(mv.size()==ml.size());
            std::cerr << " : " << mv << std::endl;
            for(size_t i=1;i<=mv.size();++i)
            {
                const COORD &sizes = mv[i];
                std::cerr << "\t\t" << sizes << std::endl;
                Partition<COORD> part(full,sizes);
                size_t items=0;
                for(size_t j=0;j<part.size();++j)
                {
                    std::cerr << "\t\t\t" << part[j] << std::endl;
                    items += part[j].items;
                }
                Y_ASSERT(items==full.items);
                std::cerr << "\t\tmaxItems=" << part.maxItems << std::endl;
            }
        }
        
    }
    
    
}

Y_UTEST(partition)
{
    string args = "3:3:3";
    if(argc>1)
    {
        args = argv[1];
    }
    doTest<Coord1D>(args);
    doTest<Coord2D>(args);
    doTest<Coord3D>(args);
    
}
Y_UTEST_DONE()



