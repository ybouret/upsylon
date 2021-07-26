#include "y/counting/symm-indx.hpp"
#include "y/yap.hpp"
#include "y/utest/run.hpp"
#include "y/utest/timings.hpp"
#include "y/mkl/types.hpp"
#include <cstdio>
using namespace upsylon;


namespace
{
    template <typename T>
    static inline void doSQRT()
    {
        for(T n=0;n<=25;++n)
        {
            const T s = mkl::sqrt_of(n);
            std::cerr << "sqrt(" << n << ")=" << s << std::endl;
        }
    }
}

Y_UTEST(isqrt)
{
    
    std::cerr << "-- Testing SymmIndx" << std::endl;
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


    std::cerr << "-- Testing a Little Bit" << std::endl;
    for(uint64_t i=1;i<limit_of<uint32_t>::maximum ; i += 1+alea.leq(1024) )
    {
        const uint32_t s = uint32_t(isqrt::of(i));
        Y_ASSERT(s*s<=i);
    }

    doSQRT<size_t>();
    doSQRT<apn>();
    doSQRT<apz>();
    doSQRT<float>();
    doSQRT<double>();
    
}
Y_UTEST_DONE()

