#include "y/math/fcn/derivative.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace math;

Y_UTEST(drvs)
{
    derivative<double> drvs;
    double err=0;
    double (*proc)(double) = sin;
    ios::ocstream fp("drvs.dat");
    for(double x=0;x<=3;x+=0.1)
    {
        const double dd = drvs.compute(proc, x, 0.1, err);
        fp("%g %g %g %g\n", x, proc(x), dd, err);
    }
}
Y_UTEST_DONE()
