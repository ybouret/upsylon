#include "y/random/marsaglia.hpp"
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
    random::cstdbits ran;
    check_bits(ran);
    check_bits(alea);
    
    random::Marsaglia::test();
}
Y_UTEST_DONE()


