#include "y/mpl/rational.hpp"
#include "y/utest/run.hpp"
#include "y/code/arith.hpp"

using namespace upsylon;

#define ITER (1<<10)

Y_UTEST(mpq)
{
    std::cerr << "-- mpq setup" << std::endl;
    for(size_t iter=0;iter<ITER;++iter)
    {
        const int64_t  n = alea.partial<int64_t>( alea.leq(62) ) * alea.pm();
        uint64_t d = 1+alea.partial<uint64_t>( alea.leq(62) );
        uint64_t a = abs_of(n);
        const mpq Q(n,d);
        arithmetic::simplify(a,d);
        //std::cerr << "Q=" << Q << " <= " << a << "/" << d << std::endl;
        Y_ASSERT(Q.num.n==a);
        Y_ASSERT(Q.den==d);
    }

    std::cerr << "-- mpq comparisons" << std::endl;
    for(size_t iter=0;iter<ITER;++iter)
    {
        const int64_t  a = alea.partial<int64_t>(30) * alea.pm();
        const uint64_t b = alea.partial<uint32_t>();
        if(b<=0) continue;

        const int64_t  c = alea.partial<int64_t>(30) * alea.pm();
        const uint64_t d = alea.partial<uint32_t>();
        if(d<=0) continue;

        const mpq      P(a,b);
        const mpq      Q(c,d);
        //std::cerr << a << "/" << b << " => " << P << std::endl;
        //std::cerr << c << "/" << d << " => " << Q << std::endl;
        const int64_t l = a*d;
        const int64_t r = b*c;
        {
            const mpz A = a;
            const mpn B = b;
            const mpz C = c;
            const mpn D = d;
            Y_ASSERT(A==a);
            Y_ASSERT(B==b);
            Y_ASSERT(C==c);
            Y_ASSERT(D==d);
            const mpz AD = A * D;
            //std::cerr << "AD=" << AD << " : " << a*d << std::endl;
            Y_ASSERT(AD==a*d);
            const mpz CB = C*B;
            //std::cerr << "CB=" << CB << " : " << c*b << std::endl;
            Y_ASSERT(CB==mpl::integer_t(c*b));

        }

        const int64_t cmp64 = l-r;
        //std::cerr << "c64=" << cmp64 << std::endl;
        const int     cmp = sign_of(cmp64);
        //std::cerr << "cmp=" << cmp << std::endl;
        const int     CMP = mpq::compare(P,Q);
        //std::cerr << "CMP=" << CMP << std::endl;
        Y_ASSERT(cmp==CMP);
    }

    std::cerr << "-- mpq additions" << std::endl;
    for(size_t iter=0;iter<ITER;++iter)
    {
        const int64_t  a = alea.partial<int64_t>(24);
        const uint64_t b = 1+alea.partial<uint64_t>(20);
        const int64_t  c = alea.partial<int64_t>(24);
        const uint64_t d = 1+alea.partial<uint64_t>(20);

        const mpq P(a,b);
        const mpq Q(c,d);
        const mpq S0(a*d+c*b,b*d);
        const mpq S1 = P + Q;
        Y_ASSERT(S0==S1);
    }
    std::cerr << "-- mpq loop++" << std::endl;
    for( mpq i(1,2); i<=10; i++)
    {
        std::cerr << i << ':';
    }
    std::cerr << std::endl;
    std::cerr << "-- mpq ++loop" << std::endl;
    for( mpq i(1,2); i<=10; ++i)
    {
        std::cerr << i << ':';
    }
    std::cerr << std::endl;
    
    std::cerr << "-- mpq subtraction" << std::endl;
    for(size_t iter=0;iter<ITER;++iter)
    {
        const int64_t  a = alea.partial<int64_t>(24);
        const uint64_t b = 1+alea.partial<uint64_t>(20);
        const int64_t  c = alea.partial<int64_t>(24);
        const uint64_t d = 1+alea.partial<uint64_t>(20);
        
        const mpq P(a,b);
        const mpq Q(c,d);
        const mpq D0(a*d-c*b,b*d);
        const mpq D1 = P - Q;
        Y_ASSERT(D0==D1);
    }
    std::cerr << "-- mpq loop--" << std::endl;
    for( mpq i(1,2); i>=-10; i--)
    {
        std::cerr << i << ':';
    }
    std::cerr << std::endl;
    std::cerr << "-- mpq --loop" << std::endl;
    for( mpq i(1,2); i>=-10; --i)
    {
        std::cerr << i << ':';
    }
    std::cerr << std::endl;
    
    std::cerr << "-- mpq multiplication" << std::endl;
    for(size_t iter=0;iter<ITER;++iter)
    {
        const int64_t  a = alea.partial<int64_t>(24);
        const uint64_t b = 1+alea.partial<uint64_t>(20);
        const int64_t  c = alea.partial<int64_t>(24);
        const uint64_t d = 1+alea.partial<uint64_t>(20);
        
        const mpq P(a,b);
        const mpq Q(c,d);
        const mpq M0(a*c,b*d);
        const mpq M1 = P * Q;
        Y_ASSERT(M0==M1);
    }
    for(mpq i=1;i.to_real()>=0.01;i*=mpq(1,2))
    {
        std::cerr << i << ':';
    }
    std::cerr << std::endl;
    
}
Y_UTEST_DONE()

