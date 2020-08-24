
#include "y/yap/prime/ratio.hpp"
#include "y/yap/prime/iterator.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/icstream.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;
using namespace yap;


namespace {



}
#include "y/string/convert.hpp"

Y_UTEST(yap_pr)
{
    prime_ratio q(100,20);
    std::cerr << q << std::endl;
}
Y_UTEST_DONE()

