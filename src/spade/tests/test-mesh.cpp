#include "y/spade/mesh/rectilinear.hpp"
#include "y/utest/run.hpp"


using namespace upsylon;
using namespace Spade;

namespace {

    template <typename COORD, typename T>
    static inline void doTest()
    {
        const COORD         rng = 30 * Coord::Ones<COORD>();
        const Layout<COORD> L( Coord::Integer(rng,alea), Coord::Integer(rng,alea) );

        RectilinearMesh<COORD,T> rmesh( "rmesh", L );
        std::cerr << rmesh.name << " : " << rmesh.category() << std::endl;

     }

}

Y_UTEST(mesh)
{

    doTest<Coord1D,float>();
    doTest<Coord1D,double>();
    doTest<Coord2D,float>();
    doTest<Coord2D,double>();
    doTest<Coord3D,float>();
    doTest<Coord3D,double>();
}
Y_UTEST_DONE()


