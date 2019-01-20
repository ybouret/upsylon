#include "y/hashing/digest.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace {
    static inline
    void do_test( digest &d )
    {
        std::cerr << std::endl;
        std::cerr << "d=0x" << d << std::endl;
        d.rand();
        std::cerr << "d=0x" << d << std::endl;

        digest c(d);
        std::cerr << "c=0x" << c << std::endl;

        for(size_t sz=0; sz<=d.size+2; ++sz )
        {
            digest tmp(sz);
            tmp.rand();
            std::cerr << "\ttmp=" << tmp << std::endl;
            d=tmp;
            std::cerr << "\t  d=" << d   << std::endl;

        }

    }
}

Y_UTEST(digest)
{
    for(size_t sz=1;sz<=8;++sz)
    {
        digest d(sz);
        do_test(d);
    }

    digest d1 = digest::hex("A");    std::cerr << "d1=" << d1 << std::endl;
    digest d2 = digest::hex("AB");   std::cerr << "d2=" << d2 << std::endl;
    digest d3 = digest::hex("ABC");  std::cerr << "d3=" << d3 << std::endl;
    digest d4 = digest::hex("ABCD"); std::cerr << "d4=" << d4 << std::endl;

}
Y_UTEST_DONE()

