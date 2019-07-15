#include "y/mpl/prime-factor.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "y/mpl/mpn.hpp"

using namespace upsylon;

Y_UTEST(prime_factor)
{

    MPN::instance().createPrimes(100);

    // first loop: individual
    for(int i=1;i<argc;++i)
    {
        const mpl::prime_factors p = argv[i];
        const mpn                a = mpn::parse(argv[i]);
        const mpn                q = p.get();

        std::cerr << argv[i] << " = " << p << " (" << q << ")" << std::endl;
        Y_ASSERT(q==a);
    }

    // second loop: product
    {
        mpl::prime_factors p = 1;
        mpn                P = 1;
        std::cerr << "p=" << p << std::endl;
        for(int i=1;i<argc;++i)
        {
            const string              s = argv[i];
            const mpl::prime_factors  a = s;
            const  mpn                A = mpn::parse(s);
            p.mul_by(a);
            P *= A;
            const mpn p_value = p.get();
            std::cerr << "p=" << p <<  " (" << p_value << ")" << " (" << P << ") " << std::endl;
            Y_ASSERT(P==p_value);
        }
    }
}
Y_UTEST_DONE()

