#include "y/geometry/contour2d.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace geometry;

Y_UTEST(geom_c2d)
{
    contour2d::database pdb;

    pdb.free();

    for(size_t i=10+alea.leq(10000);i>0;--i)
    {
        const contour2d::offset     d( alea.partial<contour2d::resolution>(), alea.partial<contour2d::resolution>() );
        const contour2d::coordinate c( alea.partial<unit_t>(), alea.partial<unit_t>() );
        const contour2d::point      p( alea.to<double>(), alea.to<double>() );
        //std::cerr << "c=" << c << ", d=" << d << ", p=" << p << std::endl;
        const contour2d::identifier id(c,d);
        const contour2d::shared_point sp = new contour2d::unique_point(id,p);
        if(!pdb.insert(sp))
        {
            std::cerr << "multiple point detected!" << std::endl;
        }
    }
    std::cerr << "#points=" << pdb.size() << std::endl;
}
Y_UTEST_DONE()

