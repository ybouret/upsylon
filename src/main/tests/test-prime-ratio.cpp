
#include "y/mpl/prime/factors-ratio.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "y/mpl/mpn.hpp"

using namespace upsylon;

Y_UTEST(prime_ratio)
{

    MPN::instance().createPrimes(100);


    mpl::prime_factors_ratio Q;
    std::cerr << "Q=" << Q << std::endl;

}
Y_UTEST_DONE()

