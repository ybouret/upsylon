#include "y/yap/natural.hpp"
#include "y/utest/run.hpp"
#include "y/exceptions.hpp"
#include "y/sequence/vector.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/icstream.hpp"

#include <cerrno>

using namespace upsylon;
using namespace yap;

namespace {
    
    static inline void check_u2w(const uint64_t u)
    {
        //std::cerr << "check_u2w..." << std::endl;
        volatile uint64_t v  = u;
        volatile size_t   nw = 0;
        const word_type  *pw = natural::u2w(v,nw);
#if 0
        std::cerr << std::hex;
        std::cerr << "u: " << u << " => [" << nw << "]";
        for(size_t i=0;i<nw;++i)
        {
            std::cerr << " " << pw[i];
        }
        std::cerr << std::endl;
#endif
        uint64_t x = 0;
        for(size_t i=nw;i>0;)
        {
            x <<= word_bits;
            x |=  pw[--i];
        }
        Y_ASSERT(u==x);
    }
}

Y_UTEST(yap_n)
{
    std::cerr << "core_bits=" << core_bits << std::endl;
    std::cerr << "word_bits=" << word_bits << std::endl;
    Y_CHECK(word_bits<=core_bits/2);
    Y_CHECK((1<< word_exp2)==word_size);

    std::cerr << "-- test u2w" << std::endl;
    {
        check_u2w(0);
        for(utype u=1,s=0;s<sizeof(utype)*8;u<<=1,++s)
        {
            check_u2w(u);
        }

        for(size_t iter=0;iter<8;++iter)
        {
            const uint64_t   u  = alea.full<uint64_t>();
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
        const utype    u  = alea.full<utype>();
        const natural  n  = u;        Y_ASSERT(n.bits()<=64); Y_ASSERT(n.size()<=8); Y_ASSERT(n.wc()<=words_per_utype);
        const natural  n1 = n;        Y_ASSERT(n1==n); Y_ASSERT( !(n1!=n) );
        natural        n2; n2 = n;
        natural        n3; n3 = u;

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

    for(size_t iter=0;iter<1024;++iter)
    {

        uint64_t a = alea.full<uint64_t>();
        uint64_t b = alea.full<uint64_t>();
        while(b==a) b = alea.full<uint64_t>();
        if(a>b) cswap(a,b); Y_ASSERT(a<b);
        natural A = a;
        natural B = b;
        
        Y_ASSERT(A<B);
        Y_ASSERT(A<=B);
        Y_ASSERT(B>A);
        Y_ASSERT(B>=A);
        B=A;
        Y_ASSERT(A<=B);
        Y_ASSERT(A>=B);
    }


#if 0
#define Y_TEST_EXCP(ERR) do {\
libc::exception e(ERR,#ERR); std::cerr << e.what() << " : " << e.when() << std::endl;\
} while(false)

    Y_TEST_EXCP(EDOM);
    Y_TEST_EXCP(ERANGE);
#endif
    std::cerr << "core_bits=" <<  core_bits << std::endl;
    std::cerr << "word_bits=" <<  word_bits << std::endl;
}
Y_UTEST_DONE()

