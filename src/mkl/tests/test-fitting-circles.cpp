
#include "y/mkl/fitting/built-in/circles.hpp"
#include "y/mkl/types.hpp"

#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace mkl;
using namespace fitting;


#if 0
namespace
{
    static void fill( sample_type &s, const float xc, const float yc, const float r )
    {
        for(size_t i=3+alea.leq(30);i>0;--i)
        {
            const float phi = numeric<float>::two_pi * alea.to<float>();
            const float x   = xc + r*cos(phi)*(1.0f+0.1f*alea.symm<float>());
            const float y   = yc + r*sin(phi)*(1.0f+0.1f*alea.symm<float>());
            s.append( unit_t(x), unit_t(y) );
        }
    }
    
    static inline void save_sample(const sample_type &s, ios::ostream &fp)
    {
        for(size_t i=1;i<=s.count();++i)
        {
            const point2d<unit_t> &p = s.abscissa[i];
            fp("%ld %ld\n", long(p.x), long(p.y));
        }
    }
    
}
#endif




Y_UTEST(fitting_circles)
{

    built_in::iCircle i_circle;
    built_in::dCircle d_circle;

    const double xc = 100 * alea.symm<double>();
    const double yc = 100 * alea.symm<double>();
    const double r  = 1 + 100 * alea.to<double>();

    std::cerr << "creating with (" << xc << "," << yc << ") : " << r << std::endl;


    ios::ocstream::overwrite("cdat.dat");
    for(size_t i=3+alea.leq(30);i>0;--i)
    {
        const double phi = numeric<float>::two_pi * alea.to<double>();
        const unit_t x   = unit_t(xc + r*cos(phi)*(1.0f+0.1f*alea.symm<double>()));
        const unit_t y   = unit_t(yc + r*sin(phi)*(1.0f+0.1f*alea.symm<double>()));

        i_circle.add(x,y);
        d_circle.add(x,y);
        ios::ocstream::echo("cdat.dat", "%ld %ld\n", long(x), long(y));
    }

    point2d<double> ic,dc;
    double          ir=0,dr=0;


    i_circle.solve(ic,ir);
    d_circle.solve(dc,dr);

    std::cerr << "center: " << ic << ", radius=" << ir << std::endl;
    std::cerr << "center: " << dc << ", radius=" << dr << std::endl;


    ios::ocstream::overwrite("cfit.dat");
    const size_t N = 200;
    for(size_t i=0;i<=N;++i)
    {
        const double phi = (numeric<float>::two_pi*i)/N;
        const double x   = dc.x + dr * cos(phi);
        const double y   = dc.y + dr * sin(phi);
        ios::ocstream::echo("cfit.dat","%g %g\n",x,y);
    }



}
Y_UTEST_DONE()


