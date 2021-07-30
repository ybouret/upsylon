#include "y/code/decimal-chars.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"

using namespace upsylon;

namespace
{
    template <typename T> static inline
    void do_check()
    {
        for(size_t loop=0;loop<1000;++loop)
        {
            const T      u = alea.partial<T>(1+alea.leq(31));
            const size_t n = decimal_chars_for(u);
            const unsigned long l = static_cast<const unsigned long>(u);
            const string        s = vformat("%lu",l);
            Y_ASSERT(s.size()==n);
        }
    }
}

Y_UTEST(decimal_chars)
{
    do_check<uint8_t>();
    do_check<uint16_t>();
    do_check<uint32_t>();
    do_check<uint64_t>();
    std::cerr << std::endl;
    for(int i=1;i<argc;++i)
    {
        const unsigned long l = atol(argv[i]);
        const size_t n = decimal_chars_for(l);
        std::cerr << l << " => " << n << std::endl;
    }
}
Y_UTEST_DONE()
