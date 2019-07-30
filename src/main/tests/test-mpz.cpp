#include "y/mpl/integer.hpp"
#include "y/comparison.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

#define ITER (1<<14)

Y_UTEST(mpz)
{
    std::cerr << "-- mpz setup and compare" << std::endl;
    for(size_t iter=0;iter<ITER;++iter)
    {
        const int64_t i   = alea.partial<int64_t>() * alea.pm();
        const int64_t j   = alea.partial<int64_t>() * alea.pm();
        const int     cmp = comparison::increasing(i,j);

        const mpz I   = i; Y_ASSERT(I.lsi()==i);
        const mpz J   = j; Y_ASSERT(J.lsi()==j);
        const int CMP = mpz::compare(I,J);
        Y_ASSERT(cmp==CMP);
    }

    std::cerr << "-- mpz addition" << std::endl;
    for(size_t iter=0;iter<ITER;++iter)
    {
        const int64_t i   = alea.partial<int64_t>(61) * alea.pm();
        const int64_t j   = alea.partial<int64_t>(61) * alea.pm();
        const int64_t s   = i+j;
        const mpz I   = i;   Y_ASSERT(I.lsi()==i);
        const mpz J   = j;   Y_ASSERT(J.lsi()==j);
        const mpz S   = I+J; Y_ASSERT(S.lsi()==s);
    }
    std::cerr << "-- mpz loop++" << std::endl;
    for(mpz i=-10;i<=10;i++)
    {
        std::cerr << i << '/';
    }
    std::cerr << std::endl;
    std::cerr << "-- mpz ++loop" << std::endl;
    for(mpz i=-10;i<=10;++i)
    {
        std::cerr << i << '/';
    }
    std::cerr << std::endl;

    std::cerr << "-- mpz subtraction" << std::endl;
    for(size_t iter=0;iter<ITER;++iter)
    {
        const int64_t i   = alea.partial<int64_t>(61) * alea.pm();
        const int64_t j   = alea.partial<int64_t>(61) * alea.pm();
        const int64_t d   = i-j;
        const mpz I   = i;   Y_ASSERT(I.lsi()==i);
        const mpz J   = j;   Y_ASSERT(J.lsi()==j);
        const mpz S   = I-J; Y_ASSERT(S.lsi()==d);
    }
    std::cerr << "-- mpz loop--" << std::endl;
    for(mpz i=10;i>=-10;i--)
    {
        std::cerr << i << '/';
    }
    std::cerr << std::endl;
    std::cerr << "-- mpz --loop" << std::endl;
    for(mpz i=10;i>=-10;--i)
    {
        std::cerr << i << '/';
    }
    std::cerr << std::endl;

    std::cerr << "-- mpz multiplication" << std::endl;
    for(size_t iter=0;iter<ITER;++iter)
    {
        const int64_t i   = alea.partial<int64_t>(31) * alea.pm();
        const int64_t j   = alea.partial<int64_t>(31) * alea.pm();
        const int64_t p   = i*j;
        const mpz I   = i;   Y_ASSERT(I.lsi()==i);
        const mpz J   = j;   Y_ASSERT(J.lsi()==j);
        const mpz P   = I*J; Y_ASSERT(P.lsi()==p);
    }

    std::cerr << "-- mpz division" << std::endl;
    for(size_t iter=0;iter<ITER;++iter)
    {
        const int64_t i   = (1+alea.partial<int64_t>(61)) * alea.pm();
        const int64_t j   = (1+alea.partial<int64_t>(32)) * alea.pm();
        const int64_t q   = i/j;
        const mpz I   = i;   Y_ASSERT(I.lsi()==i);
        const mpz J   = j;   Y_ASSERT(J.lsi()==j);
        const mpz Q   = I/J;
        //std::cerr << "i=" << i << ", j=" << j << ", q=" << q << std::endl;
        //std::cerr << "I=" << I << ", J=" << J << ", Q=" << Q << std::endl;
        Y_ASSERT(Q.lsi()==q);
    }

    std::cerr << "-- mpz square roots" << std::endl;
    std::cerr << "bits:";
    for(size_t iter=0;iter<100;++iter)
    {
        const mpz z( 10 + alea.leq(80), alea);
        const mpz z2 = mpz::square_of(z);
        const mpz s  = math::sqrt_of(z2);
        std::cerr << z2.n.bits() << "/";
        //std::cerr << z << "->" << z2 << "->" << s << std::endl;
        Y_ASSERT(s.n==z.n);
    }
    std::cerr << std::endl;
    
}
Y_UTEST_DONE()
