
#include "y/mpl/prime/factors-ratio.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "y/mpl/mpn.hpp"

using namespace upsylon;

Y_UTEST(prime_ratio)
{

    MPN::instance().createPrimes(100);


    for(size_t i=1;i<=10;++i)
    {
        const size_t n = alea.leq(1000);
        const size_t d = 1 + alea.leq(1000);
        mpl::prime_factors_ratio Q(n,d);
        std::cerr << "n=" << n << ", d=" << d <<  ", Q=" << Q << std::endl;

    }


}
Y_UTEST_DONE()

