
#include "y/spade/partition.hpp"
#include "y/utest/run.hpp"
#include "y/type/spec.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;
using namespace Spade;

namespace {
    
    template <typename COORD>
    static inline void doTest()
    {
        std::cerr << std::endl;
        std::cerr << "Layout<" << type_name_of<COORD>() << ">" << std::endl;
        
        const COORD   lo = Coord::Integer( 100 * Coord::Ones<COORD>(), alea);
        const COORD   up = Coord::Integer( 100 * Coord::Ones<COORD>(), alea);
        Coord::Disp(std::cerr << "lo=",lo) << std::endl;
        Coord::Disp(std::cerr << "up=",up) << std::endl;
        
        Layout<COORD> L(lo,up);
    }
    
    
}

Y_UTEST(partition)
{
    
    doTest<Coord1D>();
    doTest<Coord2D>();
    doTest<Coord3D>();
    
}
Y_UTEST_DONE()



