#include "y/mkl/kernel/apz-det.hpp"
#include "y/mkl/kernel/apz-adj.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/mkl/tao.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;
using namespace mkl;

namespace {
 
    template <typename T>
    static inline void test_idet()
    {
        std::cerr << "for <" << type_name_of<T>() << ">" << std::endl;
        for(size_t n=1;n<=8;++n)
        {
            std::cerr << "\tn=" << n << std::endl;
            matrix<T> M(n,n);
            for(size_t i=n;i>0;--i)
            {
                for(size_t j=n;j>0;--j)
                {
                    M[i][j] = alea.range<T>(-10,10);
                }
            }
            const apz dM = apz_det(M);
            matrix<apz> aM(n,n);
            apz_adj(aM,M);
            matrix<apz> P(n,n);
            tao::mmul(P,aM,M);
            for(size_t i=n;i>0;--i)
            {
                Y_ASSERT(dM==P[i][i]);
                for(size_t j=n;j>0;--j)
                {
                    if(i!=j)
                    {
                        Y_ASSERT(0==P[i][j]);
                    }
                }
            }
        }
        
            
    }
    
}

Y_UTEST(det)
{
    test_idet<int16_t>();
    test_idet<int>();

}
Y_UTEST_DONE()

