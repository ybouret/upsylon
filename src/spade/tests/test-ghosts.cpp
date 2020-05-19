#include "y/spade/layout/swaps.hpp"
#include "y/utest/run.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;
using namespace Spade;

namespace {

    template <typename COORD> static inline
    void doTest()
    {
        std::cerr << std::endl;
        std::cerr << "Layout<" << type_name_of<COORD>() << ">" << std::endl;

        for(int iter=0;iter<8;++iter)
        {
            const COORD   lo = Coord::Integer( 20 * Coord::Ones<COORD>(), alea);
            const COORD   up = Coord::Integer( 20 * Coord::Ones<COORD>(), alea);
            
            Layout<COORD> L(lo,up);
            std::cerr << "L=" << L << std::endl;

            for(int num=0;num<8;++num)
            {
                Layout<COORD> sub( Coord::Within(lo,up,alea), Coord::Within(lo,up,alea) );
                Y_ASSERT(L.contains(sub));
                Ghost g(sub,L);
                Y_ASSERT(sub.items==g.size());
                Ghosts G(sub, sub, L);
            }


        }

    }

}


Y_UTEST(ghosts)
{
    doTest<Coord1D>();
    doTest<Coord2D>();
    doTest<Coord3D>();

}
Y_UTEST_DONE()
