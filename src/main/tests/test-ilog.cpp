#include "y/mpl/natural.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"
#include "y/math/triplet.hpp"

#include <cmath>

using namespace upsylon;

typedef  math::triplet<double> Triplet;

namespace
{
    static const Triplet coeff = {log(2.0),2.5 - 3 * log(2.0), 1.5 - 2 * log(2.0) };
    static const double  Xmax  = (12.0*log(2.0)-8.0)/(9.0-12.0*log(2));

    struct iLog
    {
        unsigned i;
        double operator()(double x) const
        {
            const double x_i = ipower(2.0,i);
            const double dx  = (x-x_i)/x_i;
            return i * coeff.a + dx - coeff.b * (dx*dx) + coeff.c * (dx*dx*dx);
        }


    };



}

Y_UTEST(ilog)
{

    std::cerr << "coeff=" << coeff << std::endl;

    iLog L;
    {
        ios::ocstream mx("deltamax.dat");
        ios::ocstream fp("logapprox.dat");
        unsigned &i = L.i;
        for( i=0; i <= 16; ++i )
        {
            const unsigned xlo = (1<<i);
            const unsigned xup = xlo<<1;
            for(unsigned x=xlo;x<=xup;++x)
            {
                const double Lx = L(x);
                fp("%u %g %g\n", x, Lx, Lx-log(x));
            }
            const double xmax = Xmax * xlo + xlo;
            const double Lmax = L(xmax);
            const double dmax = L(xmax) - log(xmax);
            mx("%g %g %g\n", xmax, Lmax, dmax);
        }
    }


}
Y_UTEST_DONE()

