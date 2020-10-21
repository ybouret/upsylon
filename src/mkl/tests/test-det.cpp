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
            matrix<T>    M(n,n);
            matrix<apz> aM(n,n);
            {
                for(size_t i=n;i>0;--i)
                {
                    for(size_t j=n;j>0;--j)
                    {
                        M[i][j] = alea.range<T>(-10,10);
                    }
                }
                apz dM = apz_det(M);
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
                if(dM!=0)
                {
                    const apz r = apz_rescale(aM,dM);
                    std::cerr << "rescaled by " << r << std::endl;
                }
            }
            M.Id();
            for(size_t i=n;i>0;--i)
            {
                M[i][i] *= 1+alea.leq(10);
            }
            std::cerr << "M =" << M << std::endl;
            {
                apz dM = apz_det(M);
                apz_adj(aM,M);
                std::cerr << "aM=" << aM << std::endl;
                std::cerr << "dM=" << dM << std::endl;
                if(dM!=0)
                {
                    const apz r = apz_rescale(aM,dM);
                    std::cerr << "rescaled by " << r << std::endl;
                    std::cerr << "aM=" << aM << std::endl;
                    std::cerr << "dM=" << dM << std::endl;
                }
            }
        }
        
        
    }
    
}

#define SHOW_MOD(a,b) test_zmod(a,b)

namespace
{
    static inline
    void test_zmod(const int a, const int b)
    {
        const int imod = a%b;
        const apz A    = a;
        const apz B    = b;
        const apz zmod = A%B;
        std::cerr << a << '%' << b << '=' << imod <<  " => " << zmod << std::endl;
        Y_ASSERT(zmod.cast_to<int>("zmod") == imod);
    }
    
}


Y_UTEST(det)
{
    
    SHOW_MOD(1,2);
    SHOW_MOD(-1,2);
    SHOW_MOD(-3,2);
    SHOW_MOD(1,-2);
    SHOW_MOD(-1,-2);
    SHOW_MOD(-3,-2);
    
    test_idet<int16_t>();
    test_idet<int>();
    
}
Y_UTEST_DONE()

