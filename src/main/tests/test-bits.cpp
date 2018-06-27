#include "y/random/bits.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace
{
    static inline void check_bits( random::bits &ran )
    {
        for(size_t i=0;i<10;++i)
        {
            std::cerr << ran.leq(100) << ' ' << ran.to<float>() << ' ' << ran.to<double>() << std::endl;
        }
    }
}

Y_UTEST(bits)
{
    srand(time(NULL));
    random::cstdbits ran;
    check_bits(ran);
}
Y_UTEST_DONE()


