
#include "y/yap/prime/iterator.hpp"
#include "y/yap/prime/miller.hpp"

#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/icstream.hpp"

using namespace upsylon;
using namespace yap;


namespace {

    static inline void do_check( const natural &n )
    {
        std::cerr << n << " => " << Miller::is_prime(n) << "...\r";
        std::cerr.flush();
    }

}

Y_UTEST(yap_pt)
{
    library          &apl = library::instance();
    prime_iterator p(apl);

    do {
        do_check(*p);
        ++p;
    } while(apl.size()<200);
    do_check(*p);
    std::cerr << std::endl;
}
Y_UTEST_DONE()

