#include "y/counting/symm-indx.hpp"
#include "y/utest/run.hpp"

#include <cstdio>
using namespace upsylon;


Y_UTEST(isqrt)
{

#if 0
    std::cerr << "Output Table..." << std::endl;
    size_t count = 0;
    for(size_t i=0;i<65536;++i)
    {
        const size_t s = isqrt::_(i);
        Y_ASSERT(s<=255);
        fprintf(stdout,"0x%02x",unsigned(s));
        if(i<65535) fprintf(stdout,",");
        if( !(++count & 15) )
        {
            count = 0;
            fprintf(stdout,"\n");
        }
        else
        {
            fprintf(stdout," ");
        }
    }
#endif

    std::cerr << "Testing Table" << std::endl;
    for(size_t i=0;i<sizeof(isqrt::table);++i)
    {
        //std::cerr << "i=" << i << std::endl;
        Y_ASSERT(isqrt::_(i)==isqrt::table[i]);
    }

    std::cerr << "Testing SymmIndx" << std::endl;
    for(size_t n=1;n<=100;++n)
    {

        const size_t kmax = n*(n+1)/2;
        //std::cerr << "n=" << n << " -> " << kmax << std::endl;
        size_t i=0,j=0;
        for(size_t k=1;k<=kmax;++k)
        {
            symm_indx(i,j,k);
            Y_ASSERT(i<=n);
            Y_ASSERT(j<=i);
        }
    }


    std::cerr << "Testing a Little Bit" << std::endl;
    for(uint64_t i=1;i<limit_of<uint32_t>::maximum ; i += 1+alea.leq(1024) )
    {
        const uint32_t s = uint32_t(isqrt::_(i));
        Y_ASSERT(s*s<=i);
    }
}
Y_UTEST_DONE()

