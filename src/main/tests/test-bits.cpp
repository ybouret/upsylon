#include "y/randomized/marsaglia.hpp"
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
    }
}

Y_UTEST(bits)
{
    randomized::cstdbits ran;
    check_bits(ran);
    check_bits(alea);
    
    Y_CHECK(randomized::Marsaglia::test());
}
Y_UTEST_DONE()


