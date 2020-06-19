#include "y/mpl/natural.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(mp_mul)
{
    const mpn lhs=10;
    const mpn rhs=20;
    const mpn p  = mpn::lmul(lhs,rhs);
    std::cerr << lhs << "*" << rhs << "=" << p << std::endl;
}
Y_UTEST_DONE()

