#include "y/oxide/layout.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace
{

    template <typename LAYOUT>
    static inline void testLayout()
    {
        typedef typename LAYOUT::coord coord;
        std::cerr << "-- Layout Dimensions=" << LAYOUT::Dimensions << std::endl;
        const LAYOUT L( Oxide::CoordOps::Integer<coord>(100,alea), Oxide::CoordOps::Integer<coord>(100,alea) );
        std::cerr << "\tL=" << L << std::endl;
        std::cerr << "\ttesting indices" << std::endl;

        for(size_t i=L.items*8;i>0;--i)
        {
            const coord          C = L.rand( alea );
            const Oxide::Coord1D I = L.indexOf(C);
            Y_ASSERT(I>=0);
            Y_ASSERT(I<Oxide::Coord1D(L.items));
        }
        
        std::cerr << "\ttesting conversion" << std::endl;
        for(size_t idx=0;idx<L.items;++idx)
        {
            const coord  C   = L.coordOf(idx);
            const size_t jdx = L.indexOf(C);
            Y_ASSERT(idx==jdx);
        }

    }

}


Y_UTEST(oxide)
{
    testLayout<Oxide::Layout1D>();
    testLayout<Oxide::Layout2D>();
    testLayout<Oxide::Layout3D>();

}
Y_UTEST_DONE()
