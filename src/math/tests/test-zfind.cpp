#include "y/math/fcn/zfind.hpp"
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
    }
}
Y_UTEST_DONE()

