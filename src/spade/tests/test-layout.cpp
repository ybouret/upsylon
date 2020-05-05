
#include "y/spade/layout.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Spade;

namespace {
    
    template <typename COORD>
    static inline
    void doTest()
    {
        const COORD   lo = Coord::Integer( 10 * Coord::Ones<COORD>(), alea);
        const COORD   up = Coord::Integer( 10 * Coord::Ones<COORD>(), alea);
        Coord::Disp(std::cerr << "lo=",lo) << std::endl;
        Coord::Disp(std::cerr << "up=",up) << std::endl;

        Layout<COORD> L(lo,up);
        std::cerr << "\tdimensions=" << L.dimensions << std::endl;
        Coord::Disp(std::cerr << "\tlower=", L.lower) << std::endl;
        Coord::Disp(std::cerr << "\tupper=", L.upper) << std::endl;
        Coord::Disp(std::cerr << "\twidth=", L.width) << std::endl;
        Coord::Disp(std::cerr << "\tpitch=", L.pitch) << std::endl;

    }
}

Y_UTEST(layout)
{
    
    doTest<Coord1D>();
    doTest<Coord2D>();
    doTest<Coord3D>();

}
Y_UTEST_DONE()
