#include "y/mpl/natural.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

#define ITERS 16384
Y_UTEST(mpn)
{
    mpn a;
    mpn b(a);
    mpn c(10);
    std::cerr << std::hex;
    std::cerr << "-- I/O with words" << std::endl;
    for(size_t iter=0;iter<ITERS;++iter)
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
        Y_ASSERT(bytes_for(x)==wb);
        Y_ASSERT(x==tmp);

        
        mpn y = x;
        const mpn::word_type z = y.lsw();
        Y_ASSERT(z==x);
    }

    std::cerr << "-- comparison with words" << std::endl;
    for(size_t iter=0;iter<ITERS;++iter)
    {
        const uint64_t l = alea.partial<mpn::word_type>();
        const uint64_t r = alea.partial<mpn::word_type>();
        const mpn      L = l;
        const mpn      R = r;
        
        Y_ASSERT(L.lsw()==l);
        Y_ASSERT(R.lsw()==r);
        
        const int cmp = comparison::increasing(l,r);
        const int CMP = mpn::compare(L,R);
        if(cmp!=CMP)
        {
            std::cerr << "l=" << l << ", r=" << r << std::endl;
            std::cerr << "cmp=" << cmp << std::endl;
            std::cerr << "CMP=" << CMP << std::endl;

            throw exception("comparison mismatch level-1");
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
    
    std::cerr << "-- additions" << std::endl;
    for(size_t iter=0;iter<ITERS;++iter)
    {
        const uint64_t l = alea.partial<mpn::word_type>(60);
        const uint64_t r = alea.partial<mpn::word_type>(60);
        const uint64_t s = l+r;
        if(s<l||s<r) continue;
        //std::cerr << "l=" << l << ", r=" << r << std::endl;
        const mpn L = l;
        const mpn R = r;
        const mpn S = L+R;
        Y_ASSERT(S.lsw()==s);
    }
    std::cerr << "++loop" << std::endl;
    for( mpn i=0;i<30;++i)
    {
        i.to_hex(std::cerr) << "/";
    }
    std::cerr << std::endl;
    std::cerr << "loop++" << std::endl;
    for( mpn i=0;i<30;i++)
    {
        i.to_hex(std::cerr) << "/";
    }
    std::cerr << std::endl;
}
Y_UTEST_DONE()

