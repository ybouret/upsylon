#include "y/core/isqrt.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;


Y_UTEST(isqrt)
{
    std::cerr << "-- isqrt" << std::endl;
    for(uint64_t i=1;i<limit_of<uint32_t>::maximum ; i += 1+alea.leq(1024) )
    {

        const uint32_t s = uint32_t(isqrt(i));
        //std::cerr << i << " -> " << s << std::endl;
        Y_ASSERT(s*s<=i);
    }
}
Y_UTEST_DONE()

