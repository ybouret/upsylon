
#include "y/mpl/mpn.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/os/rt-clock.hpp"

using namespace upsylon;

static inline
void check_primes( const size_t count )
{
    MPN  &mgr = MPN::instance();
    
    std::cerr << "Checking #" << count << " with #plist=" << mgr.plist.size() << std::endl;

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
    std::cerr << "r_tmx=" << r_tmx << " | h_tmx=" << h_tmx << std::endl;
}

Y_UTEST(mprm)
{
    MPN     &mgr = MPN::instance();

    check_primes(2048);

    mgr.createPrimes(10);
    check_primes(2048);

    mgr.createPrimes(100);
    check_primes(2048);

    mgr.createPrimes(1000);
    check_primes(2048);


}
Y_UTEST_DONE()

