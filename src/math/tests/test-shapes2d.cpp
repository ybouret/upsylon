#include "y/math/fit/fit-circle.hpp"
#include "y/math/fit/fit-conic.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/list.hpp"
#include "y/ios/ocstream.hpp"
#include "y/math/types.hpp"

using namespace upsylon;
using namespace math;

namespace
{
    typedef point2d<unit_t> point;
}

Y_UTEST(fit_circle)
{
    const double          r = 1+alea.leq(100);
    const point2d<double> center( 10 * alea.symm<double>(), 10 * alea.symm<double>() );
    list<point> points;
    for(size_t n=3+alea.leq(100);n>0;--n)
    {
        point2d<double> p = center + alea.on_circle<double,point2d>() * r;
        p.x += 0.1*r*alea.symm<double>();
        p.y += 0.1*r*alea.symm<double>();

        const point           P( unit_t(p.x), unit_t(p.y) );
        points.push_back(P);
    }

    math::fit_circle fc;
    if(fc.compute(points.begin(),points.size()))
    {
        std::cerr << "Fitted " << r << "@" << center << " to " << fc.radius << "@" << fc.xc << "," << fc.yc << std::endl;
        {
            ios::ocstream fp("circle-points.dat");
            for( list<point>::iterator i=points.begin(); i!=points.end();++i)
            {
                fp("%g %g\n", double(i->x), double(i->y) );
            }
        }
        {
            ios::ocstream fp("circle.dat");
            for(size_t i=0;i<=360;++i)
            {
                const double angle = (i*numeric<double>::pi)/180.0;
                fp("%g %g\n", fc.xc + fc.radius * cos(angle), fc.yc + fc.radius * sin(angle) );
            }
        }
    }
    else
    {
        std::cerr << "Singular Distribution!!!" << std::endl;
    }


}
Y_UTEST_DONE()

Y_UTEST(fit_conic)
{
    list<point> points;
    const double ra = 1 + alea.leq(100);
    const double rb = 1 + alea.leq(100);
    const point2d<double> center( 10 * alea.symm<double>(), 10 * alea.symm<double>() );
    for(size_t n=10;n>0;--n)
    {
        const double theta = numeric<double>::two_pi * alea.to<double>();
        point2d<double> p( ra * cos(theta), rb*sin(theta) );
        // rotation
        p += center;
        const point           P( unit_t(p.x), unit_t(p.y) );
        points.push_back(P);
    }

    fit_conic fc;
    fc.compute(points.begin(),points.size());
    

}
Y_UTEST_DONE()

