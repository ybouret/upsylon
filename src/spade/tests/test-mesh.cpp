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
        std::cerr << rmesh.name << " : " << rmesh.category() << " : " << rmesh << std::endl;
        for(unsigned dim=0;dim<rmesh.Dimensions;++dim)
        {
            std::cerr << "\t" << rmesh[dim].name << " : " << rmesh[dim] << std::endl;
            for(Coord1D i=rmesh[dim].lower;i<=rmesh[dim].upper;++i)
            {
                rmesh[dim][i] = T(i);
            }
        }
        typename Layout<COORD>::Loop loop(rmesh.lower,rmesh.upper);
        for( loop.boot(); loop.good(); loop.next() )
        {
            const COORD    C = loop.value;
            const typename RectilinearMesh<COORD,T>::Vertex v = rmesh( C );
            const T       *p = (const T *)&v;
            for(unsigned dim=0;dim<rmesh.Dimensions;++dim)
            {
                const Coord1D c = static_cast<Coord1D>(p[dim]);
                Y_ASSERT(c==Coord::Of(C,dim));
            }
        }
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


