#include "y/counting/symm-indx.hpp"
#include "y/utest/run.hpp"
#include "y/utest/timings.hpp"
#include <cstdio>
using namespace upsylon;


Y_UTEST(symm_indx)
{
    for(size_t n=1;n<=10;++n)
    {
        const size_t kmax = ( n*(n+1) )/2;
        for(size_t k=1;k<=kmax;++k)
        {
            size_t i,j;
            symm_indx::get(i,j,k);
            Y_ASSERT(1<=i); Y_ASSERT(i<=n);
            Y_ASSERT(1<=j); Y_ASSERT(j<=i);
            Y_ASSERT(i*(i-1)/2+j==k);

            size_t im,jm;
            symm_indx::getC(im,jm,k-1);
            Y_ASSERT(i-1==im);
            Y_ASSERT(j-1==jm);
        }
    }

}
Y_UTEST_DONE()

