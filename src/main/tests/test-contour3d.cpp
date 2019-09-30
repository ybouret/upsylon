#include "y/geometry/iso3d/points.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/sort/unique.hpp"


using namespace upsylon;
using namespace geometry;
using namespace Iso3D;

Y_UTEST(contour3d)
{
    vector<Coordinate> coords;
    const unit_t m = 5;
    for(size_t i=1000;i>0;--i)
    {
        const Coordinate c(alea.range<unit_t>(-m,m),
                           alea.range<unit_t>(-m,m),
                           alea.range<unit_t>(-m,m));
        coords.push_back(c);
    }
    std::cerr << "#coords=" << coords.size() << std::endl;
    unique(coords);
    std::cerr << "#coords=" << coords.size() << std::endl;


}
Y_UTEST_DONE()

