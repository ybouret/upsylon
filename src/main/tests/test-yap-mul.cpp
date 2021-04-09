#include "y/utest/run.hpp"

#include "y/yap/natural.hpp"

using namespace upsylon;

Y_UTEST(yap_mul)
{
    std::cerr << std::hex;
    for(size_t nbit=0;nbit<=100;++nbit)
    {
        apn    lhs(alea,nbit);
        apn    rhs(alea,nbit);
        apn    p0 = apn::mul(lhs,rhs);

        std::cerr << lhs << '*' << rhs << '=' << p0;

        apn p1 = apn::lmul(lhs,rhs);
        std::cerr << '/' << p1;

        std::cerr << std::endl;
        Y_ASSERT(p0==p1);
    }

}
Y_UTEST_DONE()

