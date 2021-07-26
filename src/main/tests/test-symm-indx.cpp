#include "y/counting/symm-indx.hpp"
#include "y/utest/run.hpp"
#include "y/utest/timings.hpp"
#include <cstdio>
using namespace upsylon;


Y_UTEST(symm_indx)
{
    for(size_t n=1;n<=15;++n)
    {
        std::cerr << "n=" << n << std::endl;
        const size_t kmax = (n*(n+1))/2;
        for(size_t k=1;k<=kmax;++k)
        {
            std::cerr << "\t@" << std::setw(3) << k;
            const size_t delta=8*(k-1)+1;
            const size_t i    =(1+isqrt::of(delta))/2;
            std::cerr << " | i=" << std::setw(3) << i;
            const size_t j    =k-(i*(i-1))/2;
            std::cerr << " | j=" << std::setw(3) << j;
            std::cerr << std::endl;
            Y_ASSERT(j<=i);
            Y_ASSERT(i*(i-1)/2+j==k);
        }
    }
}
Y_UTEST_DONE()

