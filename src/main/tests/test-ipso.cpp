#include "y/ipso/layout.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(ipso)
{
    for(size_t iter=0;iter<10;++iter)
    {
        const unit_t x1 = ipso::coord1D( alea.leq(100) ) - 50;
        const unit_t x2 = ipso::coord1D( alea.leq(100) ) - 50;
        ipso::layout_of<ipso::coord1D> L1(x1,x2);
        const unit_t y1 = ipso::coord1D( alea.leq(100) ) - 50;
        const unit_t y2 = ipso::coord1D( alea.leq(100) ) - 50;
        ipso::layout_of<ipso::coord2D> L2( ipso::coord2D(x1,y1), ipso::coord2D(x2,y2) );
        const unit_t z1 = ipso::coord1D( alea.leq(100) ) - 50;
        const unit_t z2 = ipso::coord1D( alea.leq(100) ) - 50;
        ipso::layout_of<ipso::coord3D> L3( ipso::coord3D(x1,y1,z1), ipso::coord3D(x2,y2,z2) );

        std::cerr << "L1=" << L1 << std::endl;
        std::cerr << "L2=" << L2 << std::endl;
        std::cerr << "L3=" << L3 << std::endl;

    }

}
Y_UTEST_DONE()


