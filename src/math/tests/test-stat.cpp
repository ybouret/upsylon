#include "y/math/stat/metrics.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(stat)
{
    math::correlation<double> corr;

    for(size_t i=2+alea.leq(100);i>0;--i)
    {
        const double x = alea.symm<double>() * 5;
        const double y = -0.1 + 4.2 *x + alea.symm<double>();
        corr.add(x,y);
        std::cerr << "corr=" << corr.compute() << std::endl;
    }


    
}
Y_UTEST_DONE()


