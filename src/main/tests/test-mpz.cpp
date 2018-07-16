#include "y/mpl/integer.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

#define ITER 10000

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
    

}
Y_UTEST_DONE()
