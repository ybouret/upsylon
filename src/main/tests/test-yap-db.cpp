#include "y/yap/prime/iterator.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"

using namespace upsylon;
using namespace yap;

Y_UTEST(yap_db)
{
    if(argc>1)
    {
        library      &apl = library::instance();
        const natural top = natural::parse(argv[1]);
        std::cerr << "--> " << top << std::endl;
        prime_iterator p(apl);
        size_t count=1;
        while(p<top)
        {
            if( 0 == (count%100) )
            {
                const size_t nw = apl.save_to("apdb.dat");
                std::cerr << ".." << count << "@" << *p << " (wrote:" << nw << ")" << std::endl;
            }
            ++p;
            ++count;
        }
        if(apl.prune()) std::cerr << "--> pruned" << std::endl;
        const size_t nw = apl.save_to("apdb.dat");
        std::cerr << std::endl;
        std::cerr << "#primes=" << apl.primes.size+2;
        if(apl.primes.size) std::cerr<< " -> " << *(apl.primes.tail);
        std::cerr<< std::endl;
        std::cerr << "written: " << nw << std::endl;
        std::cerr << *natural::instance() << std::endl;
        
        // reloading
        {
            ios::icstream fp("apdb.dat");
            apl.load(fp,0);
        }
        std::cerr << "reloaded!" << std::endl;
        std::cerr << "#primes=" << apl.primes.size+2;
        if(apl.primes.size) std::cerr<< " -> " << *(apl.primes.tail);
        std::cerr<< std::endl;

        std::cerr << *natural::instance() << std::endl;

    }
}
Y_UTEST_DONE()

