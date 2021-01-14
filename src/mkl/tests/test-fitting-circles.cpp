
#include "y/mkl/fitting/built-in/circle.hpp"
#include "y/mkl/types.hpp"

#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace mkl;
using namespace fitting;



Y_UTEST(fitting_circles)
{

    built_in::iCircle i_circle;
    built_in::dCircle d_circle;

    const double xc = 100 * alea.symm<double>();
    const double yc = 100 * alea.symm<double>();
    const double r  = 1 + 100 * alea.to<double>();

    std::cerr << "creating with (" << xc << "," << yc << ") : " << r << std::endl;


    ios::ocstream::overwrite("cdat.dat");
    vector<double> X;
    vector<double> Y;
    for(size_t i=3+alea.leq(30);i>0;--i)
    {
        const double phi = numeric<float>::two_pi * alea.to<double>();
        const unit_t x   = unit_t(xc + r*cos(phi)*(1.0f+0.1f*alea.symm<double>()));
        const unit_t y   = unit_t(yc + r*sin(phi)*(1.0f+0.1f*alea.symm<double>()));

        i_circle.add(x,y);
        d_circle.add(x,y);
        ios::ocstream::echo("cdat.dat", "%ld %ld\n", long(x), long(y));
        X.push_back(x);
        Y.push_back(y);
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

    built_in::circle          C;
    built_in::circle::ls_type ls;

    vector<double> aorg(3,0);
    vector<double> aerr(3,0);
    vector<bool>   used(3,true);

    std::cerr << std::endl;
    std::cerr << "Fitting 1" << std::endl;
    if( C(ls, X, Y, aorg, used, aerr,  built_in::circle::with_ints) )
    {
        display_variables::errors(std::cerr, NULL, *C, aorg, used, aerr);
    }

    std::cerr << std::endl;
    std::cerr << "Fitting 2" << std::endl;

    if( C(ls, X, Y, aorg, used, aerr,  built_in::circle::with_reals) )
    {
        display_variables::errors(std::cerr, NULL, *C, aorg, used, aerr);
    }






}
Y_UTEST_DONE()


