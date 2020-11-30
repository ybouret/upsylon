#include "y/yap/lcm.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/list.hpp"
#include "y/string/convert.hpp"

using namespace upsylon;
using namespace yap;

Y_UTEST(yap_lcm)
{
    typedef number::utype utype;
    list<utype>           u;
    list<apn>             U;

    for(int i=1;i<argc;++i)
    {
        u << string_convert::to<size_t>(argv[i],"value");
        U << apn::parse(argv[i]);
    }
    std::cerr <<  "u=" << u << std::endl;
    std::cerr <<  "U=" << U << std::endl;

    std::cerr << "using native  type:" << std::endl;
    apn l = lcm::of(u); std::cerr << "l=" << l << std::endl;

    std::cerr << "using natural type:" << std::endl;
    apn L = lcm::of(U); std::cerr << "L=" << L << std::endl;


}
Y_UTEST_DONE()

