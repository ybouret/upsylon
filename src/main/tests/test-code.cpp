#include "y/code/round.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(code)
{
    for(size_t i=0;i<=100;i+=3)
    {
        std::cerr << i
        << ' ' << Y_ROUND1(i)
        << ' ' << Y_ROUND2(i)
        << ' ' << Y_ROUND4(i)
        << ' ' << Y_ROUND8(i)
        << ' ' << Y_ROUND16(i)
        << ' ' << Y_ROUND32(i)
        << ' ' << Y_ROUND64(i)
        << ' ' << Y_ROUND128(i)
        << ' ' << Y_ROUND256(i)
        << std::endl;
    }
}
Y_UTEST_DONE()


