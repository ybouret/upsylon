
#include "y/yap/library.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/icstream.hpp"

using namespace upsylon;
using namespace yap;

#define ITER 128

namespace {

}

Y_UTEST(yap_p)
{
    Y_UTEST_SIZEOF(natural);
    Y_UTEST_SIZEOF(prime);

    library &apl = library::instance();

    apl.reset_primes();

}
Y_UTEST_DONE()

