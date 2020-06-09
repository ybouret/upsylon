#include "y/mkl/fcn/zfind.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace math;

namespace
{
    static inline
    double F( double x )
    {
        return 0.1+cos( x*x );
    }
}

Y_UTEST(zfind)
{
    {
        triplet<double> x = {0, -1, 2};
        triplet<double> f = {F(x.a),-1,F(x.c)};
        std::cerr << "x=" << x << std::endl;
        std::cerr << "f=" << f << std::endl;
        if(zfind::run(F,x,f))
        {
            std::cerr << "F(" << x.b << ")=" << f.b << std::endl;
        }
        else
        {
            std::cerr << "couldn't find zero" << std::endl;
        }
        const double x1 = zfind::run1(F,0.0,2.0);
        std::cerr << "x1=" << x1 << std::endl;
        const double x2 = zfind::run2(0.11,F,0.0,2.0);
        std::cerr << "x2=" << x2 << std::endl;

    }
}
Y_UTEST_DONE()

