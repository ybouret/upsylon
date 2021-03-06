#include "y/mkl/kernel/cholesky.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/mkl/tao.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;
using namespace mkl;

namespace  {
    
    template <typename T> static inline
    void do_test()
    {

        for(size_t n=1;n<=32;++n)
        {
            matrix<T> A(n,n);
            matrix<T> A0(n,n);
            vector<T> x(n);
            vector<T> b(n);
            vector<T> y(n);
            
            for(size_t iter=1;iter<=16;++iter)
            {
                const size_t m = n + 1 + alea.leq(10);
                matrix<T> P(n,m);
                for(size_t i=1;i<=n;++i)
                {
                    for(size_t j=1;j<=m;++j)
                    {
                        P[i][j] = support::get<T>()-T(0.5);
                    }
                }
                tao::mmul_rtrn(A, P, P);
                A0 = A;


                if( !cholesky::build(A) )
                {
                    std::cerr << "-";
                    continue;
                }
                //std::cerr << "A =" << A  << std::endl;


                for(size_t i=1;i<=n;++i)
                {
                    b[i] = support::get<T>() - T(0.5);
                }
                //A*x=b
                cholesky::solve(x, A, b);
                tao::mul(y, A0, x);

                
                //std::cerr << "b=" << b << ":y=" << y << std::endl;
                const T RMS = tao::rms<T>::of(b,y);
                std::cerr << '<' << RMS << '>';
            }
        }
        std::cerr << std::endl;
    }
}

Y_UTEST(cholesky)
{
    do_test<float>();
    do_test<double>();

    {
        size_t n=4;
        matrix<double> J(n,n);
    build:
        for(size_t i=1;i<=n;++i)
        {
            for(size_t j=1;j<=n;++j)
            {
                J[i][j] = alea.symm<double>();
            }
        }
        matrix<double> J2(n,n);
        tao::mmul_rtrn(J2,J,J);
        std::cerr << "J="  << J  << std::endl;
        std::cerr << "J2=" << J2 << std::endl;
        if( !cholesky::build(J2) )
            goto build;


        std::cerr << "L=" << J2 << std::endl;

    }

}
Y_UTEST_DONE()

