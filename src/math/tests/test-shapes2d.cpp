#include "y/math/fit/shapes2d.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/list.hpp"

using namespace upsylon;

namespace
{
    typedef point2d<unit_t> point;
}

Y_UTEST(shapes2d)
{
    const double          r = 1+alea.leq(100);
    const point2d<double> center( 10 * alea.symm<double>(), 10 * alea.symm<double>() );
    list<point> points;
    for(size_t n=3+alea.leq(100);n>0;--n)
    {
        const point2d<double> p = center + alea.on_circle<double,point2d>() * r;
        const point           P( unit_t(p.x), unit_t(p.y) );
        points.push_back(P);
    }

    math::fit_circle fc;
    if(fc.compute(points.begin(),points.size()))
    {
        std::cerr << "Fitted " << r << "@" << center << " to " << fc.radius << "@" << fc.xc << "," << fc.yc << std::endl;
    }

}
Y_UTEST_DONE()

