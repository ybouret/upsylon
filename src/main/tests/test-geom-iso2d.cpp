#include "y/geometry/iso2d.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace geometry;

Y_UTEST(geom_iso2d)
{
    iso2d::shared_points db;
    for(size_t iter=10+alea.leq(100);iter>0;--iter)
    {
        const unit_t   i = 100 - unit_t( alea.leq(200) );
        const unit_t   j = 100 - unit_t( alea.leq(200) );
        const unsigned p = alea.partial<unsigned>(5);
        const iso2d::identifier   tag(i,j,p);
        const iso2d::vertex       vtx = alea.on_circle<double,point2d>();
        const iso2d::shared_point sp  = new iso2d::unique_point(tag,vtx);
        if(!db.insert(sp))
        {
            continue;
        }

    }
    std::cerr << "#unique_point=" << db.size() << std::endl;
}
Y_UTEST_DONE()

