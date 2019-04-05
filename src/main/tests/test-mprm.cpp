
#include "y/mpl/natural.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"

using namespace upsylon;

Y_UTEST(mprm)
{

    MPN &mgr = MPN::instance();

    std::cerr << "primes=" << mgr.primes << std::endl;
    mgr.append_primes(1);
    std::cerr << "primes=" << mgr.primes << std::endl;
    mgr.append_primes(1);
    std::cerr << "primes=" << mgr.primes << std::endl;
    mgr.append_primes(100);
    std::cerr << "primes=" << mgr.primes << std::endl;

    if(argc>1)
    {
        ios::icstream fp(argv[1]);
        mgr.load_primes(fp);
    }

    std::cerr << "primes=" << mgr.primes << std::endl;

}
Y_UTEST_DONE()

