#include "y/randomized/marsaglia.hpp"
#include "y/randomized/isaac.hpp"
#include "y/randomized/mersenne-twister.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace
{
    static inline void check_bits( randomized::bits &ran )
    {
        for(size_t i=0;i<10;++i)
        {
            std::cerr << ran.leq(100) << ' ' << ran.to<float>() << ' ' << ran.to<double>() << std::endl;
        }
        std::cerr << std::endl;
    }
}

Y_UTEST(bits)
{
    randomized::cstdbits ran;
    randomized::ISAAC<4> isaac4;
    randomized::ISAAC<8> isaac8;
    randomized::mersenne_twister mt;

    check_bits(ran);
    check_bits(alea);
    check_bits(isaac4);
    check_bits(isaac8);
    check_bits(mt);
    check_bits(randomized::bits::simple());
    check_bits(randomized::bits::crypto());

    Y_CHECK(randomized::Marsaglia::test());
}
Y_UTEST_DONE()


