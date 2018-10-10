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
    for(size_t sz=1;sz<=3;++sz)
    {
        digest d(sz);
        do_test(d);
    }
    
}
Y_UTEST_DONE()

