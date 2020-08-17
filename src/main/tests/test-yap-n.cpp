#include "y/yap/natural.hpp"
#include "y/utest/run.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

using namespace upsylon;
using namespace yap;

Y_UTEST(yap_n)
{
    std::cerr << "core_bits=" << number::core_bits << std::endl;
    std::cerr << "word_bits=" << number::word_bits << std::endl;
    Y_CHECK(number::word_bits<=number::core_bits/2);
    Y_CHECK((1<<number::word_exp2)==number::word_size);

    std::cerr << "-- test zero" << std::endl;
    {
        natural zero;
        std::cerr << "zero=" << zero << std::endl;
        {
            natural tmp(200,as_capacity);
            zero.xch(tmp);
        }
        std::cerr << "zero=" << zero << std::endl;
    }


    std::cerr << "-- test set/lsw" << std::endl;
    for(size_t iter=0;iter<1024;++iter)
    {
        const uint64_t u = alea.full<uint64_t>();
        const natural  n = u;
        const natural  n1 = n;
        natural        n2;
        n2 = n;
        natural        n3;
        n3 = u;
        Y_ASSERT(n.lsw()==u);
        Y_ASSERT(n1.lsw()==u);
        Y_ASSERT(n2.lsw()==u);
        Y_ASSERT(n3.lsw()==u);

    }

    std::cerr << "-- test ran" << std::endl;
    for(size_t iter=0;iter<1024;++iter)
    {
        const size_t  bits = alea.leq(5000);
        const natural n(alea,bits);
        Y_ASSERT(n.bits()==bits);
    }


#if 0
#define Y_TEST_EXCP(ERR) do {\
libc::exception e(ERR,#ERR); std::cerr << e.what() << " : " << e.when() << std::endl;\
} while(false)

    Y_TEST_EXCP(EDOM);
    Y_TEST_EXCP(ERANGE);
#endif
    std::cerr << "core_bits=" << number::core_bits << std::endl;
    std::cerr << "word_bits=" << number::word_bits << std::endl;
}
Y_UTEST_DONE()

