#include "y/yap/natural.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/icstream.hpp"


using namespace upsylon;
using namespace yap;

namespace {
    
    static inline void check_u2w(const uint64_t u)
    {
        volatile uint64_t v  = u;
        volatile size_t   nw = 0;
        const natural::word_type  *pw = natural::u2w(v,nw);

        number::utype x = 0;
        for(size_t i=nw;i>0;)
        {
            x <<= natural::word_bits;
            x |=  pw[--i];
        }
        Y_ASSERT(u==x);
    }
}

Y_UTEST(yap_n)
{
    std::cerr << "core_bits=" << natural::core_bits << std::endl;
    std::cerr << "word_bits=" << natural::word_bits << std::endl;
    Y_CHECK(natural::word_bits<=natural::core_bits/2);
    Y_CHECK((1<< natural::word_exp2)==natural::word_size);

    std::cerr << "-- test u2w" << std::endl;
    {
        check_u2w(0);
        for(number::utype u=1,s=0;s<sizeof(number::utype)*8;u<<=1,++s)
        {
            check_u2w(u);
        }

        for(size_t iter=0;iter<8;++iter)
        {
            const number::utype u  = alea.full<number::utype>();
            check_u2w(u);
        }
    }
    std::cerr << std::endl;

    std::cerr << "-- test zero" << std::endl;
    {
        natural zero;
        std::cerr << "zero=" << zero << std::endl;

        Y_ASSERT(0==zero);
        Y_ASSERT(zero==0);
        Y_ASSERT( !(0!=zero) );
        Y_ASSERT( !(zero!=0) );

        {
            natural tmp(200,as_capacity);
            zero.xch(tmp);
        }
        std::cerr << "zero=" << zero << std::endl;
    }
    std::cerr << std::endl;



    std::cerr << "-- test set/lsw" << std::endl;
    for(size_t iter=0;iter<1024;++iter)
    {
        const number::utype u  = alea.full<number::utype>();
        const natural       n  = u; Y_ASSERT(n.bits()<=64); Y_ASSERT(n.size()<=8); Y_ASSERT(n.wc()<=natural::words_per_utype);
        const natural       n1 = n; Y_ASSERT(n1==n); Y_ASSERT( !(n1!=n) );
        natural             n2; n2 = n;
        natural             n3; n3 = u;

        Y_ASSERT(n.lsw() ==u);
        Y_ASSERT(n1.lsw()==u);
        Y_ASSERT(n2.lsw()==u);
        Y_ASSERT(n3.lsw()==u);
        Y_ASSERT(n==u);
        Y_ASSERT(u==n);
        Y_ASSERT(!(n!=u));
        Y_ASSERT(!(u!=n));
    }
    std::cerr << std::endl;

    std::cerr << "-- test ran/eq" << std::endl;
    {
        std::cerr << std::dec;
        vector<natural> rans;
        size_t          written = 0;
        {
            ios::ocstream fp("apn-ran.dat");
            for(size_t iter=0;iter<1024;++iter)
            {
                const size_t  bits = alea.leq(5000);
                const natural n(alea,bits);
                Y_ASSERT(n.bits()==bits);
                rans.push_back(n);
                written += n.serialize(fp);
            }
        }
        std::cerr << "\t#written = " << written << std::endl;

        {
            ios::icstream fp("apn-ran.dat");
            size_t        read  = 0;
            size_t        count = 0;
            while( fp.is_active() )
            {
                size_t delta = 0;
                const  natural n = natural::read(fp, delta, "apn");
                read += delta;
                ++count;
                if(count>rans.size()) throw exception("too many apn in file");
                Y_ASSERT( n == rans[count] );
                Y_ASSERT( ! (n!=rans[count]) );
            }
            std::cerr << "\t#read    = " << read << std::endl;
        }
    }
    std::cerr << std::endl;



    std::cerr << "-- test comparison" << std::endl;
    {
        size_t count=0;
        for(size_t lbits=0;lbits<=1024;lbits += 1+alea.leq(10))
        {
            const natural lhs(alea,lbits);
            for(size_t rbits=0;rbits<lbits;rbits+=1+alea.leq(10))
            {
                const natural rhs(alea,rbits);
                Y_ASSERT(rhs<lhs);
                Y_ASSERT(lhs>rhs);
                ++count;
            }
            Y_ASSERT(lhs<=lhs);
            Y_ASSERT(lhs>=lhs);
            Y_ASSERT(lhs==lhs);
            Y_ASSERT(!(lhs!=lhs));
        }
        std::cerr << "|_done with at least " << count << " trials..." << std::endl;
    }

    for(size_t iter=0;iter<1024;++iter)
    {
        const number::utype a = alea.full<number::utype>();
        const number::utype b = alea.full<number::utype>();
        const natural A=a;
        const natural B=b;
        if(a<b)
        {
            Y_ASSERT(A<B); Y_ASSERT(A<=B);
            Y_ASSERT(a<B); Y_ASSERT(a<=B);
            Y_ASSERT(A<b); Y_ASSERT(A<=b);
        }
        else
        {
            if(a>b)
            {
                Y_ASSERT(A>B); Y_ASSERT(A>=B);
                Y_ASSERT(a>B); Y_ASSERT(a>=B);
                Y_ASSERT(A>b); Y_ASSERT(A>=b);
            }
        }
    }

    std::cerr << "-- test addition" << std::endl;
    for(size_t iter=0;iter<1024;++iter)
    {
        const uint64_t a = alea.partial<uint64_t>(63);
        const uint64_t b = alea.partial<uint64_t>(63);
        const uint64_t c = a+b;
        const natural  A = a; Y_ASSERT( A.lsw() == a );
        const natural  B = b; Y_ASSERT( B.lsw() == b );
        const natural  C = c; Y_ASSERT( C.lsw() == c );
        const natural  S = A+B;
        Y_ASSERT(S==C);
    }

    for(size_t iter=0;iter<1024;++iter)
    {
        const natural A(alea,alea.leq(5000));
        {
            const natural ID = +A; Y_ASSERT(ID==A);
        }
        const natural B(alea,alea.leq(5000));
        const natural C = A+B;
        {
            natural  a = A;
            a += B;
            Y_ASSERT(C==a);
        }
        {
            natural b = B;
            b += A;
            Y_ASSERT(C==b);
        }
    }

    for(size_t iter=0;iter<1024;++iter)
    {
        const natural  A(alea,alea.leq(5000));
        const uint64_t b = alea.full<uint64_t>();
        const natural  B = b;
        const natural  S = A+B;
        assert( A+b == S );
        assert( b+A == S );
        {
            natural a  = A;
            a += b;
            Y_ASSERT(S==a);
        }
    }

    for(size_t iter=0;iter<16;++iter)
    {
        {
            natural       i(alea,alea.leq(5000));
            const size_t  n = alea.leq(1000);
            const natural end = i+n;
            size_t        count = 0;
            for(;i<end;++i)
            {
                ++count;
            }
            Y_ASSERT(n==count);
        }

        {
            natural       i(alea,alea.leq(5000));
            const size_t  n = alea.leq(1000);
            const natural end = i+n;
            size_t        count = 0;
            for(;i<end;i++)
            {
                ++count;
            }
            Y_ASSERT(n==count);
        }

    }


    std::cerr << *natural::instance() << std::endl;



}
Y_UTEST_DONE()

