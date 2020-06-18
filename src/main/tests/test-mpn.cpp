#include "y/mpl/mpn.hpp"
#include "y/code/arith.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/code/primality.hpp"
#include "y/ios/ocstream.hpp"
#include "y/code/utils.hpp"
#include "y/comparison.hpp"
#include "y/string.hpp"
#include "y/type/utils.hpp"

using namespace upsylon;

#define ITERS       (1<<11)
#define ITERS_SMALL (1<<8)

namespace {

    template <typename T>
    static inline void test_convert()
    {
        std::cerr << std::dec;
        std::cerr << "|_test_convert<uint" << sizeof(T) * 8 << "_t>" << std::endl;
        {
            mpn zero;
            T   ans=0;
            Y_ASSERT(zero.to<T>(ans));
            Y_ASSERT(0==ans);
        }

        for(size_t iter=0;iter<16*024*1024;++iter)
        {
            const T   x = alea.full<T>();
            const mpn n = x;
            T         y = alea.full<T>();
            Y_ASSERT(n.to<T>(y));
            Y_ASSERT(x==y);
        }
    }
}

Y_UTEST(mpn)
{

    if(false)
    {
        ios::ocstream fp("miller.dat");
        for(size_t i=1;i<=10000;++i)
        {
            const double x = double(i);
            const double l = 2*square_of(log(x));
            fp("%g %g %g\n", x,l,ceil(l));
        }
    }

    MPN &mgr = MPN::instance();

    mpn a = 0;
    mpn b(a);
    mpn c(10);

    {
        mpn aa("12",as_string);
        mpn bb("0x10",as_string);
        const string value = "0x123";
        mpn cc = value;
        cc = "0x321";
    }

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

    std::cerr << "-- words conversion" << std::endl;
    test_convert<uint8_t>();
    test_convert<uint16_t>();
    test_convert<uint32_t>();
    test_convert<uint64_t>();


    
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
        const mpn L = l;
        const mpn R = r;
        const mpn S = L+R;
        Y_ASSERT(S.lsw()==s);
    }
    std::cerr << std::dec;
    std::cerr << "++loop" << std::endl;
    for( mpn i;i<30;++i)
    {
        std::cerr << i << "/";
    }
    std::cerr << std::endl;
    std::cerr << "loop++" << std::endl;
    for( mpn i;i<30;i++)
    {
        std::cerr << i << "/";
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
        std::cerr << i << "/";
    }
    std::cerr << std::endl;
    std::cerr << "loop--" << std::endl;
    for( mpn i=30;i>0;i--)
    {
        std::cerr << i << "/";
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

    std::cerr << "-- divisibility" << std::endl;
    {
        for(uint64_t num=0;num<=1024;++num)
        {
            const mpn Num = num;
            for( uint64_t den=1; den<=1024+256;++den )
            {
                const mpn  Den = den;
                const bool res = 0 == (num%den);
                Y_ASSERT( res == Num.is_divisible_by(Den) );
            }
        }
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
        if(r>0)
        {
            arithmetic::simplify(l,r);
            arithmetic::simplify(L,R);
            Y_ASSERT(L.lsw()==l);
            Y_ASSERT(R.lsw()==r);
        }
    }

    std::cerr << "-- I/O" << std::endl;
    {
        for(size_t i=0;i<1000;i+=1+alea.leq(30))
        {
            const mpn I = i;
            std::cerr << std::hex << "hex: i=" << i << " => " << I << std::endl;
            std::cerr << std::dec << "dec: i=" << i << " => " << I << std::endl;
            std::cerr << "\treal: " << I.to_real() << std::endl;
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
        mpn i;
        while(P.size()<=100)
        {
            i = mgr.nextPrime(i+1);
            P.push_back(i);
        }
        std::cerr << "P=" << P << std::endl;
    }

    std::cerr << "-- rsa" << std::endl;
    std::cerr << std::hex;
    for(size_t iter=0;iter<4;++iter)
    {
        std::cerr << "generating p..." << std::endl;
        mpn p( 5+alea.leq(25), randomized::bits::crypto() );
        p = mgr.nextPrime(p);
        std::cerr << "p=" << p << std::endl;
        std::cerr << "generating q..." << std::endl;
        mpn q( 5+alea.leq(25), randomized::bits::crypto() );
        q = mgr.nextPrime(q);
        std::cerr << "q=" << q << std::endl;
        const mpn n = p*q;
        std::cerr << "n=" << p*q << std::endl;
        const mpn phi = (p-1)*(q-1);
        std::cerr << "phi=" << phi << std::endl;
        mpn  e( phi.bits()/2, randomized::bits::crypto() );
        e = mgr.nextPrime(e);
        std::cerr << "e=" << e << std::endl;
        const mpn d = mpn::mod_inv(e,phi);
        std::cerr << "d=" << d << std::endl;

        for(size_t i=0;i<4;++i)
        {
            const mpn M(alea.lt(n.bits()),randomized::bits::simple());
            const mpn C = mpn::mod_exp(M,e,n);
            const mpn D = mpn::mod_exp(C,d,n);
            std::cerr << "M=" << M << "->" << C << "->" << D << " : ";
            if(D==M)
            {
                std::cerr << "success" << std::endl;
            }
            else
            {
                std::cerr << "failure!" << std::endl;
            }
        }

    }

    std::cerr << "-- input hex" << std::endl;
    for(size_t iter=0;iter<ITERS_SMALL;++iter)
    {
        const size_t ns=1+alea.leq(30);
        string h;
        for(size_t i=0;i<ns;++i)
        {
            h << hexadecimal::lowercase_word[ alea.lt(16) ];
        }
        std::cerr << std::dec << "h=  " << h << std::endl;
        const mpn n = mpn::hex(h);
        std::cerr << std::hex << "n=" << n << std::endl;
    }

    std::cerr << "-- input dec" << std::endl;
    for(size_t iter=0;iter<ITERS_SMALL;++iter)
    {
        const size_t ns=1+alea.leq(30);
        string       d;
        std::cerr << std::dec;
        for(size_t i=0;i<ns;++i)
        {
            d << char('0'+alea.lt(10));
        }
        std::cerr << std::dec << "d=" << d << std::endl;
        const mpn n = mpn::dec(d);
        std::cerr << std::dec << "n=" << n << std::endl;
        d.skip_with('0');
        if(d.size()<=0) d='0';
        const string D = n.to_decimal();
        std::cerr << "D=" << D << "/" << d << std::endl;
        Y_ASSERT(D==d);
    }

    std::cerr << "-- square root" << std::endl;

    std::cerr << "bits:" << std::endl;
    for(size_t iter=100;iter>0;--iter)
    {
        const mpn n( 10 + alea.leq(80), alea );
        const mpn n2 = mpn::square_of(n);
        const mpn s  = mkl::sqrt_of(n2);
        std::cerr << n2.bits() << "/";
        Y_ASSERT(s==n);
    }
    std::cerr << std::endl;

    std::cerr << "sizeof(mpn)=" << sizeof(mpn) << std::endl;
    
}
Y_UTEST_DONE()

Y_UTEST(mpn_fact)
{
    for(int i=1;i<argc;++i)
    {
        const string s = argv[i];
        const mpn    n = s;
        const mpn    f = mpn::factorial(n);
        std::cerr << "factorial(" << n << ")=" << f << std::endl;
    }
}
Y_UTEST_DONE()



