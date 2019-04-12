
#include "y/mpl/mpn.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/os/rt-clock.hpp"
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

Y_UTEST(mprm)
{
    size_t n = 100;
    if(argc>1)
    {
        n = string_convert::to<size_t>(argv[1],"n");
    }
    MPN     &mgr = MPN::instance();

    check_primes(n);

    mgr.createPrimes(10);
    check_primes(n);

    mgr.createPrimes(100);
    check_primes(n);

    mgr.createPrimes(1000,MPN::CreateFast);
    check_primes(n);

    
}
Y_UTEST_DONE()

#include "y/utest/timings.hpp"

static inline uint64_t mpn_check(MPN &mgr, const size_t num_iter)
{
    const mpn      &n     = mgr.upper();
    uint64_t        count = 0;
    for(mpn i=0;i<=n;++i)
    {
        mpn np = i;
        Y_ASSERT(mgr.locateNextPrime(np));
        size_t iter = num_iter;
        while(iter-->0)
        {
            uint64_t ini = rt_clock::ticks();
            Y_ASSERT(mgr.isComputedPrime(np));
            count += rt_clock::ticks() - ini;
        }
    }
    return count;
}

Y_UTEST(mprm2)
{
    size_t n = 100;
    if(argc>1)
    {
        n = string_convert::to<size_t>(argv[1],"n");
    }
    MPN      &mgr = MPN::instance();
    rt_clock tmx;
    const string filename = "mprm.dat";
    ios::ocstream::overwrite(filename);

    const size_t iter = 256;
    while( mgr.primes() < n )
    {
        ios::ocstream fp(filename,true);
        const double speed = 1e-6*mgr.primes()/(tmx(mpn_check(mgr,iter))/iter);
        fp("%u %g\n", unsigned(mgr.primes()), speed);
        std::cerr << mgr.primes() << " => " << speed << std::endl;
        mgr.createPrimes(1,MPN::CreateFast);
    }

}
Y_UTEST_DONE()
