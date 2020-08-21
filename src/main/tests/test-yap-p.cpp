
#include "y/yap/library.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/icstream.hpp"
#include "y/sequence/vector.hpp"
#include "y/os/real-time-clock.hpp"
#include "y/os/progress.hpp"

using namespace upsylon;
using namespace yap;

#define ITER 128

namespace {



}
#include "y/string/convert.hpp"

Y_UTEST(yap_p)
{
    Y_UTEST_SIZEOF(natural);
    Y_UTEST_SIZEOF(prime);

    library &apl = library::instance();
    size_t    n = 2000;
    if(argc>1)
    {
        n=string_convert::to<size_t>(argv[1],"n");
    }
    apl.reset_primes();

    vector<natural> P(n,as_capacity);
    progress        bar;

    std::cerr << "Computing First " << n << " primes..." << std::endl;
    uint64_t t0 = 0;
    {
        bar.start();
        natural p = 0;
        bar.print(std::cerr,0,0);
        for(size_t i=1;i<=n;++i)
        {
            const uint64_t mark = real_time_clock::ticks();
            p = apl.next_prime_(p);
            t0 += real_time_clock::ticks()-mark;
            P.push_back(p);
            //bar.update(i,n);
            bar.print(std::cerr,i,n,0.5);
        }
        std::cerr << std::endl;
    }

    std::cerr << "Prefetching to sqrt of " << P.back() << std::endl;
    while( apl.prefetch().squared < P.back() )
    {

    }
    std::cerr << "Computed #" << apl.primes.size << " extra primes" << " up to " << *apl.primes.tail << "^2=" << apl.primes.tail->squared <<  std::endl;
    {
        const size_t written = apl.save_to("apl.dat");
        std::cerr << "#written=" << written << std::endl;
    }

    std::cerr << "Computing First " << n << " primes v2..." << std::endl;
    uint64_t t1 = 0;
    {
        bar.start();
        natural p = 0;
        bar.print(std::cerr,0,0);
        for(size_t i=1;i<=n;++i)
        {
            const uint64_t mark = real_time_clock::ticks();
            p = apl.next_prime_(p);
            t1 += real_time_clock::ticks()-mark;
            Y_ASSERT(p==P[i]);
            bar.print(std::cerr,i,n,0.5);
        }
        std::cerr << std::endl;
    }

    std::cerr << "t0: " << t0 << std::endl;
    std::cerr << "t1: " << t1 << std::endl;











}
Y_UTEST_DONE()

