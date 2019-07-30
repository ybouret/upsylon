#include "y/math/fit/in2d/circle.hpp"
#include "y/math/fit/in2d/ellipse.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/list.hpp"
#include "y/ios/ocstream.hpp"
#include "y/math/types.hpp"
#include "y/math/kernel/tao.hpp"
#include "y/math/trigconv.hpp"

using namespace upsylon;
using namespace math;

namespace
{
    typedef point2d<unit_t> point;
}

Y_UTEST(fit_circle)
{
    const double          r = double(1+alea.leq(100));
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

    Fit::Circle fc;

    if( fc.compute(points.begin(),points.size()) )
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

}
Y_UTEST_DONE()

Y_UTEST(fit_conic)
{
    list<point> points;
    const double ra = 1 + double(alea.leq(100));
    const double rb = 1 + double(alea.leq(100));
    const point2d<double> center( 10 * alea.symm<double>(), 10 * alea.symm<double>() );
    std::cerr << "center=" << center << std::endl;
    std::cerr << "ra    =" << ra     << std::endl;
    std::cerr << "rb    =" << rb     << std::endl;
    const double phi = alea.to<double>() * numeric<double>::pi;

    {
        ios::ocstream fp("conic-points.dat");
        matrix<double> R(2,2);
        R[1][1] = R[2][2] = cos(phi);
        R[1][2] = -(R[2][1] = sin(phi));
        std::cerr << "phi=" << phi << std::endl;
        std::cerr << "R  =" << R   << std::endl;
        for(size_t n=10;n>0;--n)
        {
            const double theta = numeric<double>::two_pi * alea.to<double>();
            point2d<double> q( ra * cos(theta), rb*sin(theta) );
            // rotation
            point2d<double> p = q.mul_by(R);
            // translation
            p += center;
            const point           P( unit_t(p.x), unit_t(p.y) );
            points.push_back(P);
            fp("%g %g\n", center.x, center.y);
            fp("%g %g\n\n", double(points.back().x), double(points.back().y));
        }
    }

    {
        std::cerr << "Generic Conic..." << std::endl;
        Fit::Conic fc;
        if(fc.compute(points.begin(),points.size()))
        {
            std::cerr << "q=" << fc.q << std::endl;
        }
        else
        {
            std::cerr << "failure" << std::endl;
        }

    }

    {
        std::cerr << "Ellipse..." << std::endl;
        Fit::Ellipse fc;
        if( fc.compute(points.begin(),points.size()) )
        {
            std::cerr << "q=" << fc.q << std::endl;
            fc.factorize();
            std::cerr << "center=" << fc.center << std::endl;
            std::cerr << "radii =" << fc.radii  << std::endl;
            const matrix<double> &R = fc.rotation();
            std::cerr << "rotation=" << R << std::endl;
            const double angle = angle_of(R[1][1], R[2][1]);
            std::cerr << "angle=" << rad2deg(angle) << " / phi=" << rad2deg(phi) << ", delta=" << rad2deg(fabs_of(angle-phi)) << std::endl;

            ios::ocstream fp("ellipse.dat");
            for(double a=0; a<= 6.3; a += 0.01 )
            {
                point2d<double> p( fc.radii.x * cos(a), fc.radii.y * sin(a) );
                p = p.mul_by(fc.rotation());
                p += fc.center;
                fp("%g %g\n", p.x, p.y );
            }

        }
        else
        {
            std::cerr << "failure" << std::endl;
        }
    }

    }
    Y_UTEST_DONE()

