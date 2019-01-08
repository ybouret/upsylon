#include "y/randomized/sobol.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;

Y_UTEST(sobol)
{
    int n=-1;
    double v[2];
    randomized::sobol::compute(n,NULL);
    n=2;
    {
        ios::ocstream fp("sobol2.dat");
        for(size_t i=1000;i>0;--i)
        {
            randomized::sobol::compute(n,v-1);
            fp("%g %g\n", v[0], v[1]);
        }
    }

}
Y_UTEST_DONE()

