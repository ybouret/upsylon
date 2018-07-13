#include "y/mpl/natural.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

#define ITERS (1<<14)

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

    std::cerr << "-- bits with words" << std::endl;
    for(size_t iter=0;iter<ITERS;++iter)
    {
        const mpn::word_type x  = alea.partial<mpn::word_type>();
        const mpn X = x;
        Y_ASSERT(X.bits()==bits_for(x));
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

    std::cerr << "-- subtraction" << std::endl;
    for(size_t iter=0;iter<ITERS;++iter)
    {
        uint64_t l = alea.partial<mpn::word_type>();
        uint64_t r = alea.partial<mpn::word_type>();
        if(l<r) cswap(l,r);
        assert(l>=r);
        const uint64_t d = l-r;
        const mpn L = l;
        const mpn R = r;
        const mpn D = L-R;
        Y_ASSERT(D.lsw()==d);
    }
    std::cerr << "--loop" << std::endl;
    for( mpn i=30;i>0;--i)
    {
        i.to_hex(std::cerr) << "/";
    }
    std::cerr << std::endl;
    std::cerr << "loop--" << std::endl;
    for( mpn i=30;i>0;i--)
    {
        i.to_hex(std::cerr) << "/";
    }
    std::cerr << std::endl;

    std::cerr << "-- multiplication" << std::endl;
    for(size_t iter=0;iter<ITERS;++iter)
    {
        const uint64_t l = alea.full<uint32_t>();
        const uint64_t r = alea.full<uint32_t>();
        const uint64_t p = l*r;
        const mpn L = l;
        const mpn R = r;
        const mpn P = L*R;
        Y_ASSERT(P.lsw()==p);
        const uint64_t l2 = l*l;
        const mpn L2 = mpn::square_of(L);
        Y_ASSERT(L2.lsw()==l2);
    }

    std::cerr << "-- left bit shifting" << std::endl;
    for(size_t iter=0;iter<ITERS;++iter)
    {
        const uint64_t l = alea.full<uint32_t>();
        const mpn      L = l;
        Y_ASSERT(bits_for(l)==L.bits());
        for(size_t shift=0;shift<32;++shift)
        {
            const uint64_t ls = l << shift;
            const mpn      LS = L << shift;
            Y_ASSERT(LS.lsw()==ls);
        }
    }

    std::cerr << "-- right bit shifting" << std::endl;
    for(size_t iter=0;iter<ITERS;++iter)
    {
        const uint64_t l = alea.full<uint64_t>();
        const mpn      L = l;
        Y_ASSERT(bits_for(l)==L.bits());
        for(size_t shift=0;shift<64;++shift)
        {
            const uint64_t ls = l >> shift;
            const mpn      LS = L >> shift;
            Y_ASSERT(LS.lsw()==ls);
        }
    }

    std::cerr << "-- power of two" << std::endl;
    for(size_t j=0;j<100;++j)
    {
        const mpn L = mpn::exp2(j);
        if(j<64)
        {
            Y_ASSERT( L.lsw() == (uint64_t(1)<<j));
        }
    }

    std::cerr << "-- divisions" << std::endl;
    for(size_t iter=0;iter<ITERS;++iter)
    {
        const uint64_t num = alea.partial<uint64_t>();
        const uint64_t den = 1+alea.partial<uint64_t>(60);
        const uint64_t q   = num/den;
        const mpn Num = num;
        const mpn Den = den;
        const mpn Q   = Num/Den;
        Y_ASSERT(Q.lsw()==q);
    }

}
Y_UTEST_DONE()

