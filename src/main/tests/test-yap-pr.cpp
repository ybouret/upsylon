
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
    for(size_t i=0;i<10;++i)
    {
        const natural       num(alea,alea.leq(15));
        std::cerr << "num=" << num << std::endl;
        const prime_factors pfn = num;
        std::cerr << "pfn=" << pfn << std::endl;

        const natural       den(alea,1+alea.lt(15));
        std::cerr << "den=" << den << std::endl;
        const prime_factors pfd = den;
        std::cerr << "pfd=" << pfd << std::endl;
        const prime_ratio   q(pfn,pfd);
        std::cerr << "[" << num << "=" << pfn << "]/[" << den << "=" << pfd << "]";
        std::cerr.flush();
        std::cerr << "=" << q << std::endl;
    }
}
Y_UTEST_DONE()

