
#include "y/spade/layout.hpp"
#include "y/utest/run.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;
using namespace Spade;

namespace {
    
    template <typename COORD>
    static inline
    void doTest()
    {
        std::cerr << std::endl;
        std::cerr << "Layout<" << type_name_of<COORD>() << ">" << std::endl;
        const COORD   lo = Coord::Integer( 100 * Coord::Ones<COORD>(), alea);
        const COORD   up = Coord::Integer( 100 * Coord::Ones<COORD>(), alea);
        Coord::Disp(std::cerr << "lo=",lo) << std::endl;
        Coord::Disp(std::cerr << "up=",up) << std::endl;

        Layout<COORD> L(lo,up);
        std::cerr << "\tdimensions=" << L.dimensions << std::endl;
        Coord::Disp(std::cerr << "\tlower=", L.lower) << std::endl;
        Coord::Disp(std::cerr << "\tupper=", L.upper) << std::endl;
        Coord::Disp(std::cerr << "\twidth=", L.width) << std::endl;
        Coord::Disp(std::cerr << "\tpitch=", L.pitch) << std::endl;
        std::cerr << L << std::endl;
       
        std::cerr << "\tindex->coord" << std::endl;
        for(Coord1D i=0;i<Coord1D(L.items);++i)
        {
            const COORD q = L.coordOf(i);
            Y_ASSERT(L.has(q));
        }

        std::cerr << "\tcoord->index" << std::endl;
        typename Layout<COORD>::Loop loop(L.lower,L.upper);
        for(loop.boot();loop.good();loop.next())
        {
            const Coord1D i = L.indexOf(loop.value);
            const COORD   q = L.coordOf(i);
            Y_ASSERT(loop.value==q);
        }
        
    }
}

Y_UTEST(layout)
{
    
    doTest<Coord1D>();
    doTest<Coord2D>();
    doTest<Coord3D>();

}
Y_UTEST_DONE()
