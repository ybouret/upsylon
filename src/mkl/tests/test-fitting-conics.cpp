

#include "y/mkl/fitting/built-in/conics.hpp"
#include "y/mkl/types.hpp"

#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace mkl;
using namespace fitting;


Y_UTEST(fitting_conics)
{

    built_in::iConics IC;
    built_in::iConics DC;

    IC.ellipse();
    DC.ellipse();

    const double ra = 10 + 100 * alea.to<double>();
    const double rb = (0.1+0.85*alea.to<double>()) * ra;
    const double xc = 200 * alea.symm<double>();
    const double yc = 200 * alea.symm<double>();

    std::cerr << "xc=" << xc << std::endl;
    std::cerr << "yc=" << yc << std::endl;

    ios::ocstream::overwrite("edat.dat");
    for(size_t i=10+alea.leq(10);i>0;--i)
    {
        const double phi = numeric<double>::two_pi * alea.to<double>();
        const double x0  = ra * cos(phi);
        const double y0  = rb * sin(phi);
        const double x   = xc + x0;
        const double y   = yc + y0;
        const unit_t X   = unit_t(x);
        const unit_t Y   = unit_t(y);
        ios::ocstream::echo("edat.dat","%ld %ld\n", long(X), long(Y) );
        IC.add(X,Y);
    }



    if( IC.build_shape() )
    {
        std::cerr << "built shape" << std::endl;
        IC.find_values();
    }

}
Y_UTEST_DONE()

