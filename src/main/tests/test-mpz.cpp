#include "y/mpl/integer.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

#define ITER 16384

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
        const mpz S   = I*J; Y_ASSERT(S.lsi()==p);
    }
}
Y_UTEST_DONE()
