#include "y/utest/run.hpp"
#include "y/yap/natural.hpp"

using namespace upsylon;

Y_UTEST(yap_mul)
{
    apn lhs;
    apn rhs;
    apn prod = apn::mul(lhs,rhs);

    std::cerr << lhs << '*' << rhs << '=' << prod << std::endl;
}
Y_UTEST_DONE()

