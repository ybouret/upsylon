#include "y/mpl/natural.hpp"
#include "y/code/arith.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/code/primality.hpp"

using namespace upsylon;

#define ITERS (1<<14)
#define ITERS_SMALL (1<<10)
Y_UTEST(mpn)
{
    mpn a;
    mpn b(a);
    mpn c(10);
    std::cerr << std::hex;
    std::cerr << "-- I/O with words" << std::endl;
    for(size_t iter=0;iter<ITERS;++iter)
    {
        const uint64_t   x  = alea.partial<uint64_t>();
        uint64_t         w  = x;
        size_t           wb = 0;
        const uint8_t   *p  = mpn::prepare(w,wb);
        uint64_t         tmp = 0;
        for(size_t i=0;i<wb;++i)
        {
            tmp <<= 8;
            tmp |= p[(wb-i)-1];
        }
        Y_ASSERT(bytes_for(x)==wb);
        Y_ASSERT(x==tmp);

        
        mpn y = x;
        const uint64_t z = y.lsw();
        Y_ASSERT(z==x);
    }

    std::cerr << "-- bits with words" << std::endl;
    for(size_t iter=0;iter<ITERS;++iter)
    {
        const uint64_t x  = alea.partial<uint64_t>();
        const mpn X = x;
        Y_ASSERT(X.bits()==bits_for(x));
    }

    std::cerr << "-- comparison with words" << std::endl;
    for(size_t iter=0;iter<ITERS;++iter)
    {
        const uint64_t l = alea.partial<uint64_t>();
        const uint64_t r = alea.partial<uint64_t>();
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
        const uint64_t l = alea.partial<uint64_t>(60);
        const uint64_t r = alea.partial<uint64_t>(60);
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
        uint64_t l = alea.partial<uint64_t>();
        uint64_t r = alea.partial<uint64_t>();
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

    std::cerr << "-- divisions/modulos" << std::endl;
    for(size_t iter=0;iter<ITERS;++iter)
    {
        const uint64_t num = alea.partial<uint64_t>();
        const uint64_t den = 1+alea.partial<uint64_t>(60);
        const uint64_t q   = num/den;
        const uint64_t r   = num%den;
        const mpn Num = num;
        const mpn Den = den;
        const mpn Q   = Num/Den;
        const mpn R   = Num%Den;
        Y_ASSERT(Q.lsw()==q);
        Y_ASSERT(R.lsw()==r);
    }

    std::cerr << "-- boolean" << std::endl;
    for(size_t iter=0;iter<ITERS;++iter)
    {
        const uint64_t l = alea.partial<uint64_t>();
        const uint64_t r = alea.partial<uint64_t>();
        const uint64_t a = l&r;
        const uint64_t o = l|r;
        const uint64_t x = l^r;

        const mpn L = l;
        const mpn R = r;
        const mpn A = L&R;
        const mpn O = L|R;
        const mpn X = L^R;
        Y_ASSERT(A.lsw()==a);
        Y_ASSERT(O.lsw()==o);
        Y_ASSERT(X.lsw()==x);

    }

    std::cerr << "-- arithmetic" << std::endl;

    for(size_t iter=0;iter<ITERS_SMALL;++iter)
    {
        uint64_t l = alea.partial<uint64_t>();
        uint64_t r = alea.partial<uint64_t>();
        const uint64_t g = arithmetic::gcd(l,r);

        mpn L = l;
        mpn R = r;
        const mpn G = arithmetic::gcd(L,R);
        Y_ASSERT(G.lsw()==g);
        arithmetic::simplify(l,r);
        arithmetic::simplify(L,R);
        Y_ASSERT(L.lsw()==l);
        Y_ASSERT(R.lsw()==r);
    }

    std::cerr << "-- I/O" << std::endl;
    {
        for(size_t i=0;i<1000;i+=1+alea.leq(30))
        {
            const mpn I = i;
            std::cerr << std::hex << "hex: i=" << i << " => " << I << std::endl;
            std::cerr << std::dec << "dec: i=" << i << " => " << I << std::endl;
        }
    }

    std::cerr << "-- primality" << std::endl;
    vector<uint64_t> p;
    {
        size_t i=0;
        while(p.size()<=100)
        {
            i = primality::next(i+1);
            p.push_back(i);
        }
        std::cerr << "p=" << p << std::endl;
    }
    vector<mpn> P;
    {
        mpn i=0;
        while(P.size()<=100)
        {
            i = mpn::next_prime(i+1);
            P.push_back(i);
        }
        std::cerr << "P=" << P << std::endl;
    }

}
Y_UTEST_DONE()

