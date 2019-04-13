
#include "y/mpl/mpn.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/utest/timings.hpp"

#include <cmath>

using namespace upsylon;

static inline
void check_primes( const size_t count )
{
    MPN  &mgr = MPN::instance();
    
    std::cerr << "Checking #" << count << " with #plist=" << mgr.plist.size() << std::endl;
    std::cerr << "\tchecking locateNextPrime..." << std::endl;
    for(mpn i=0;i<=mgr.plist.back().p;++i)
    {
        mpn np = i;
        Y_ASSERT(mgr.locateNextPrime(np));
        //std::cerr << i << " -> " << np << std::endl;
    }
    std::cerr << "\tchecking performances..." << std::endl;
    mpn p=0,q=0;
    uint64_t r_ticks = 0;
    uint64_t h_ticks = 0;

    for(size_t i=1;i<=count;++i)
    {
        {
            const uint64_t mark = rt_clock::ticks();
            p = mgr.nextPrime_(++p);
            r_ticks += (rt_clock::ticks()-mark);
        }
        {
            const uint64_t mark = rt_clock::ticks();
            q = mgr.nextPrime(++q);
            h_ticks += (rt_clock::ticks()-mark);
        }
        Y_ASSERT(p==q);
    }
    rt_clock rtc;
    const double r_tmx = rtc(r_ticks);
    const double h_tmx = rtc(h_ticks);
    std::cerr << "\t\tr_tmx=" << r_tmx << " | h_tmx=" << h_tmx << std::endl;
    const double speed_up = floor( 1000.0*(h_tmx-r_tmx)/r_tmx + 0.5)/10.0;
    std::cerr << "\t\tgain : " << -speed_up << "%" << std::endl;
}

#include "y/string/convert.hpp"

static inline
void check_consistency(const size_t count,
                       const size_t cycle)
{
    assert(cycle>0);
    MPN &mgr = MPN::instance();
    mgr.reset();

    const string filename = "mpchk.dat";
    ios::ocstream::overwrite(filename);

    rt_clock rtc;

    for(;mgr.plist.size()<=count;mgr.createPrimes(1))
    {
        std::cerr << "#primes=" << mgr.plist.size() << " -> [" << mgr.plist.back().p << "]" << std::endl;
        const mpn n      = mgr.plist.back().p;
        uint64_t  count1 = 0;
        uint64_t  count2 = 0;
        for(size_t j=0;j<cycle;++j)
        {
            for(mpn i=0;i<=n;++i)
            {
                mpn np = i;
                Y_TIMINGS_TICKS(count1,Y_ASSERT(mgr.locateNextPrime(np)) );
                Y_TIMINGS_TICKS(count2,Y_ASSERT(mgr.isComputedPrime(np)) );
            }
        }
        const double factor = 1e6 / (mgr.plist.size()*cycle);
        ios::ocstream::echo(filename, "%u %g %g\n", unsigned(mgr.plist.size()), factor * rtc(count1), factor * rtc(count2) );
    }


}

Y_UTEST(mprm)
{
    size_t count = 500;
    size_t cycle = 1;

    if(argc>1)
    {
        count = string_convert::to<size_t>(argv[1],"count");
    }

    if(argc>2)
    {
        cycle = string_convert::to<size_t>(argv[2],"cycle");
    }

    std::cerr << "-- checking consistency with #primes=" << count << " and #cycles=" << cycle << std::endl;
    check_consistency(count,cycle);
    return 0;

#if 0
    MPN     &mgr = MPN::instance();

    check_primes(n);

    mgr.createPrimes(10);
    check_primes(n);

    mgr.createPrimes(100);
    check_primes(n);

    mgr.createPrimes(1000,MPN::CreateFast);
    check_primes(n);
#endif

    
}
Y_UTEST_DONE()


