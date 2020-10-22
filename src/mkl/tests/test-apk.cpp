#include "y/mkl/kernel/apk.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/mkl/tao.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;
using namespace mkl;

namespace {
    
    template <typename T>
    static inline void test_idet(const size_t nmax)
    {
        std::cerr << "for <" << type_name_of<T>() << ">" << std::endl;
        for(size_t n=1;n<=nmax;++n)
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
                apz dM = apk::determinant(M);
                apk::adjoint(aM,M);
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
                    apz r = 0;
                    apk::simplify(aM,dM,&r);
                    std::cerr << "rescaled by " << r << std::endl;
                    if(r>1)
                    {
                        std::cerr << "aM=" << aM << std::endl;
                        std::cerr << "dM=" << dM << std::endl;
                    }
                }
            }
            M.Id();
            for(size_t i=n;i>0;--i)
            {
                M[i][i] *= 1+alea.leq(10);
            }
            std::cerr << "M =" << M << std::endl;
            {
                apz dM = apk::determinant(M);
                apk::adjoint(aM,M);
                std::cerr << "aM=" << aM << std::endl;
                std::cerr << "dM=" << dM << std::endl;
                if(dM!=0)
                {
                    apz r = 0;
                    apk::simplify(aM,dM,&r);
                    std::cerr << "rescaled by " << r << std::endl;
                    std::cerr << "aM=" << aM << std::endl;
                    std::cerr << "dM=" << dM << std::endl;
                }
            }
        }
        
    }


    template <typename T>
    static inline void test_adjoint_gram(const size_t n)
    {
        std::cerr << "gram/adjoint for <" << type_name_of<T>() << ">" << std::endl;

        const size_t m = n + alea.leq(5);
        matrix<T>    M(n,m);
        for(size_t i=1;i<=n;++i)
        {
            for(size_t j=1;j<=m;++j)
            {
                M[i][j] = alea.range<T>(-5,5);
            }
        }

        std::cerr << "M=" << M << std::endl;
        matrix<apz> A(n,n);
        apz         d = apk::adjoint_gram(A,M);
        std::cerr << "d=" << d << std::endl;
        std::cerr << "A=" << A << std::endl;

        matrix<int64_t> I(n,n);
        apk::convert(I,A);
        std::cerr << "I=" << I << std::endl;

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


Y_UTEST(apk)
{
    
    SHOW_MOD(1,2);
    SHOW_MOD(-1,2);
    SHOW_MOD(-3,2);
    SHOW_MOD(1,-2);
    SHOW_MOD(-1,-2);
    SHOW_MOD(-3,-2);
    
    test_idet<int16_t>(6);
    test_idet<int>(8);

    test_adjoint_gram<int>(6);

}
Y_UTEST_DONE()

