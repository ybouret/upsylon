#include "y/mkl/kernel/ideterminant.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"

using namespace upsylon;
using namespace mkl;

namespace {
 
    template <typename T>
    static inline void test_idet()
    {
        for(size_t n=1;n<=10;++n)
        {
            matrix<T> M(n,n);
            for(size_t i=n;i>0;--i)
            {
                for(size_t j=n;j>0;--j)
                {
                    M[i][j] = alea.range<T>(-10,10);
                }
            }
            const apz dM = ideterminant(M);
            std::cerr << "det(" << M << ")"<< std::endl;
            std::cerr << dM << std::endl;
        }
        
            
    }
    
}

Y_UTEST(det)
{
    test_idet<int16_t>();
}
Y_UTEST_DONE()

