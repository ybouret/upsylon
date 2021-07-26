#include "y/counting/symm-indx.hpp"
#include "y/utest/run.hpp"
#include "y/utest/timings.hpp"
#include <cstdio>
using namespace upsylon;


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

#if 0
    std::cerr << "-- Testing Speed: Raw..." << std::endl;
    double speed_raw = 0;
    Y_TIMINGS(speed_raw,1.0,
              for(size_t i=0;i<65536;++i) { (void)isqrt::_(i); }
              );
    std::cerr << "\t" << speed_raw << std::endl;
    
    std::cerr << "-- Testing Speed: Opt..." << std::endl;
    double speed_opt = 0;
    Y_TIMINGS(speed_opt,1.0,
              for(size_t i=0;i<65536;++i) { (void)isqrt::of(i); }
              );
    std::cerr << "\t" << speed_opt << std::endl;
#endif
    
}
Y_UTEST_DONE()

