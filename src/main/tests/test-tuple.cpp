#include "y/container/tuple.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace
{
    Y_PAIR_DECL(STANDARD,dummy2,int,v1,unsigned,v2);
    Y_PAIR_END();
}

Y_UTEST(tuple)
{
    dummy2 d2a(1,2);
    dummy2 d2b(1,3);
    Y_CHECK(d2a!=d2b);
    Y_CHECK(!(d2a==d2b));
    std::cerr << "d2a=" << d2a << std::endl;
    std::cerr << "d2b=" << d2b << std::endl;
}
Y_UTEST_DONE()

