#include "y/mpl/natural.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(mpn)
{
    mpn a;
    mpn b(a);
    mpn c(10);

    std::cerr << std::hex;
    for(size_t iter=0;iter<128;++iter)
    {
        const mpn::word_type x  = alea.choice() ? alea.full<uint32_t>() : alea.full<uint64_t>();
        mpn::word_type       w  = x;
        size_t               wb = 0;
        const uint8_t       *p  = mpn::prepare(w,wb);
        mpn::word_type       tmp = 0;
        for(size_t i=0;i<wb;++i)
        {
            tmp <<= 8;
            tmp |= p[(wb-i)-1];
        }
        if(bytes_for(x)!=wb)
        {
            std::cerr << x <<"->" << tmp << std::endl;
            std::cerr << "bytes: " << bytes_for(x) << "/" << wb << std::endl;
            throw exception("invalid mpn::prepare level-1");
        }

        if(x!=tmp)
        {
            std::cerr << x <<"->" << tmp << std::endl;
            throw exception("invalid mpn::prepare level-2");
        }
    }

}
Y_UTEST_DONE()

