#include "y/randomized/sobol.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;

Y_UTEST(sobol)
{
    double v[2];
    randomized::sobol s;
    {
        ios::ocstream fp("sobol2.dat");
        for(size_t i=1000;i>0;--i)
        {
            s.compute(2,v);
            fp("%g %g\n", v[0], v[1]);
        }
    }

}
Y_UTEST_DONE()

