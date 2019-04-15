
#include "y/mpl/mpn.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/utest/timings.hpp"
#include "y/string/convert.hpp"

#include <cmath>

using namespace upsylon;


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

}
Y_UTEST_DONE()

#include "y/sequence/vector.hpp"
#include "y/string/tokenizer.hpp"

static inline bool isSep( int C )
{
    return C==' ' || C=='\t';
}

Y_UTEST(mprm1000)
{
    if(argc<=1) return 0;

    vector<mpn> pv(1000,as_capacity);
    {
        std::cerr << "-- Loading " << argv[1] << std::endl;
        ios::icstream fp(argv[1]);
        string line;
        vector<string,memory::pooled> words(64,as_capacity);
        while(fp.gets(line))
        {
            tokenizer<char>::split(words,line,isSep);
            for(size_t i=1;i<=words.size();++i)
            {
                const mpn j = mpn::dec(words[i]);
                pv.push_back(j);
            }
        }
    }
    std::cerr << "-- Loaded " << pv.size() << " values" << std::endl;
    const size_t np = pv.size();
    std::cerr << "-- Creating primes" << std::endl;
    MPN &mp = MPN::instance();
    mp.createPrimes( max_of<size_t>(np,2)-2 ); assert(mp.mpvec.size>=np);
    std::cerr << "-- Checking primes" << std::endl;
    for(size_t i=1;i<=np;++i)
    {
        const mpn &lhs = *mp.mpvec.slot[i];
        const mpn &rhs = pv[i];
        if( lhs != rhs )
        {
            std::cerr << "Error in prime list: " << lhs << " != " << rhs << std::endl;
            throw exception("primes mismatch");
        }
    }
    std::cerr << "-- All done..." << std::endl;
}
Y_UTEST_DONE()
