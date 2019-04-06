
#include "y/mpl/natural.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"

using namespace upsylon;

Y_UTEST(mprm)
{

    MPN &mgr = MPN::instance();
    std::cerr << "plist=" << mgr.plist << std::endl;
    std::cerr << "probe=" << mgr.probe << std::endl;

    for(size_t iter=1;iter<=4;++iter)
    {
        mgr.createPrimes(1);
        std::cerr << "plist=" << mgr.plist << std::endl;
        std::cerr << "probe=" << mgr.probe << std::endl;
    }

    mgr.createPrimes(20);
    std::cerr << "plist=" << mgr.plist << std::endl;
    std::cerr << "probe=" << mgr.probe << std::endl;

    if(argc>1)
    {
        ios::icstream fp(argv[1]);
        mgr.reloadPrimes(fp);
        std::cerr << "plist=" << mgr.plist << std::endl;
        std::cerr << "probe=" << mgr.probe << std::endl;
    }



}
Y_UTEST_DONE()

