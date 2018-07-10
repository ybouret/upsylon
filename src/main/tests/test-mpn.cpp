#include "y/mpl/natural.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(mpn)
{
    mpn a;
    mpn b(a);
    mpn c(10);

    std::cerr << "-- I/O with words" << std::endl;
    for(size_t iter=0;iter<10000;++iter)
    {
        const mpn::word_type x  = alea.partial<mpn::word_type>();
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

    std::cerr << "-- comparison with words" << std::endl;
    for(size_t iter=0;iter<10000;++iter)
    {
        const uint64_t l = alea.partial<mpn::word_type>();
        const uint64_t r = alea.partial<mpn::word_type>();
        const mpn      L = l;
        const mpn      R = r;

        const int cmp = comparison::increasing(l,r);
        const int CMP = mpn::compare(L,R);
        if(cmp!=CMP)
        {
            throw exception("comparison mismatch");
        }
        if(0!=mpn::compare(L,L))
        {
            throw exception("comparison error");
        }

        if(cmp!=comparison::increasing(L,R))
        {
            throw exception("comparison mismatch level-2");
        }
    }


}
Y_UTEST_DONE()

