#include "y/mpl/prime-factor.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(prime_factor)
{
    for(int i=1;i<argc;++i)
    {
        const mpl::prime_factors p = argv[i];
        const mpn                a = mpn::parse(argv[i]);
        const mpn                q = p.get();

        std::cerr << argv[i] << " = " << p << " (" << q << ")" << std::endl;
        Y_ASSERT(q==a);
        
    }
}
Y_UTEST_DONE()

