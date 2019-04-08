
#include "y/mpl/natural.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;

Y_UTEST(mprm)
{

    MPN &mgr = MPN::instance();
    std::cerr << "plist=" << mgr.plist << std::endl;
    std::cerr << "probe=" << mgr.probe << std::endl;

    double max_slope = 0;


    const size_t len0 = mgr.recordLength();
    const size_t num0 = mgr.plist.size();

    {
        ios::ocstream fp("prm.dat");
        fp("%u %u\n", unsigned(num0), unsigned(len0) );
        std::cerr << num0 << " -> " << len0 << "@" << mgr.plist.back().p << "/" << mgr.probe.p << std::endl;



        for(unsigned i=1;i<=1000;++i)
        {
            mgr.createPrimes(1);
            const size_t num = mgr.plist.size();
            const size_t len = mgr.recordLength();
            fp("%u %u\n", unsigned(num), unsigned(len) );
            std::cerr << num << " -> " << len << "@" << mgr.plist.back().p << "/" << mgr.probe.p << std::endl;
            max_slope = max_of(max_slope, double(len-len0)/double(num-num0));
        }

    }
    std::cerr << '#' << num0 << " -> " << len0 << std::endl;

    const double intercept = double(len0) - max_slope * double(num0);

    std::cerr << "max_slope=" << max_slope << std::endl;
    std::cerr << "intercept=" << intercept << std::endl;

    {
        ios::ocstream fp("prm.bin");
        mgr.recordPrimes(fp);
    }

}
Y_UTEST_DONE()

