
#include "y/ipso/fields.hpp"
#include "y/ipso/ghosts.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/sequence/vector.hpp"
#include "y/ipso/field3d.hpp"

using namespace upsylon;
using namespace ipso;

Y_UTEST(ipso_gh)
{
    fields F;

    const unit_t x1 = ipso::coord1D( alea.leq(100) ) - 50;
    const unit_t x2 = ipso::coord1D( alea.leq(100) ) - 50;
    layout1D L1(x1,x2);

    {
        F.add< field1D<double> >( "f1d", L1 );
        F.add< field1D<string> >( "f1s", L1 );

    }

    const unit_t y1 = ipso::coord1D( alea.leq(100) ) - 50;
    const unit_t y2 = ipso::coord1D( alea.leq(100) ) - 50;
    ipso::layout_of<ipso::coord2D> L2( ipso::coord2D(x1,y1), ipso::coord2D(x2,y2) );

    {
        F.add< field2D<double>  >( "f2d", L2 );
        F.add< field2D<uint8_t> >( "f2u", L2 );
    }

    const unit_t z1 = ipso::coord1D( alea.leq(100) ) - 50;
    const unit_t z2 = ipso::coord1D( alea.leq(100) ) - 50;
    ipso::layout_of<ipso::coord3D> L3( ipso::coord3D(x1,y1,z1), ipso::coord3D(x2,y2,z2) );

    {
        F.add< field3D<float>  >( "f3f", L3 );
        F.add< field3D<int>    >( "f2i", L3 );
    }

    for( fields::iterator i=F.begin();i!=F.end();++i)
    {
        const field_info &f = **i;
        std::cerr << "-- " << f.name << std::endl;
    }
}
Y_UTEST_DONE()


