
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

    {
        mpl::prime_factors_ratio Q(0,10);
        std::cerr << "Q.num=" << Q.num << ", Q.den=" << Q.den << std::endl;
        Y_ASSERT(Q.den.is_one());
    }

    mpl::prime_factors_ratio Q = 1;
    std::cerr << "Q=" << Q << std::endl;
    for(size_t i=1;i<=10;++i)
    {
        const size_t n = 1+alea.leq(1000);
        Q.mul_by(n);
        std::cerr << "*=" << n << " => " << Q << std::endl;
        const size_t d = 1+alea.leq(1000);
        Q.div_by(d);
        std::cerr << "/=" << d << " => " << Q << std::endl;
    }

    // permutation
    std::cerr << "Permutations: " << std::endl;
    for(mpn n;n<=16;++n)
    {
        std::cerr << "perm(" << n << "):";
        for(mpn k;k<=n;++k)
        {
            const mpl::prime_factors np = mpl::prime_factors::perm(n,k);
            const mpn                hp = np.get();
            std::cerr << " " << k << ":" << hp;
        }
        std::cerr << std::endl;
    }
    // permutation
    std::cerr << "Combinations: " << std::endl;
    for(mpn n;n<=16;++n)
    {
        std::cerr << "comb(" << n << "):";
        for(mpn k;k<=n;++k)
        {
            const mpl::prime_factors np = mpl::prime_factors::comb(n,k);
            const mpn                hp = np.get();
            std::cerr << " " << k << ":" << hp;
        }
        std::cerr << std::endl;
    }


}
Y_UTEST_DONE()

