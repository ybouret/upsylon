

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
    built_in::dConics DC;

    IC.ellipse();
    DC.ellipse();

    const double ra = 10 + 100 * alea.to<double>();
    const double rb = 10 + 100 * alea.to<double>();
    const double xc = 200 * alea.symm<double>();
    const double yc = 200 * alea.symm<double>();

    const double th = numeric<double>::two_pi * alea.to<double>();
    const double cc = cos(th);
    const double ss = sin(th);


    std::cerr << "xc=" << xc << std::endl;
    std::cerr << "yc=" << yc << std::endl;

    std::cerr << "ra=" << ra << std::endl;
    std::cerr << "rb=" << rb << std::endl;
    std::cerr << std::endl;
    
    ios::ocstream::overwrite("edat.dat");
    for(size_t i=10+alea.leq(10);i>0;--i)
    {
        const double phi = numeric<double>::two_pi * alea.to<double>();
        const double x0  = ra * cos(phi);
        const double y0  = rb * sin(phi);
        const double x   = xc + cc*x0 - ss * x0;
        const double y   = yc + ss*x0 + cc * y0;
        const unit_t X   = unit_t(x);
        const unit_t Y   = unit_t(y);
        ios::ocstream::echo("edat.dat","%ld %ld\n", long(X), long(Y) );
        IC.add(X,Y);
        DC.add(double(X),double(Y));
    }



    if( IC.build_shape() )
    {
        std::cerr << "i-built shape" << std::endl;
        IC.find_values();
    }
    
    if( DC.build_shape() )
    {
        std::cerr << "d-built shape" << std::endl;
        DC.find_values();
    }

}
Y_UTEST_DONE()

