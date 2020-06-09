#include "y/mkl/kernel/cholesky.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/mkl/kernel/quark.hpp"
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
                quark::mmul_rtrn(A, P, P);
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
                quark::mul(y, A0, x);

                
                //std::cerr << "b=" << b << ":y=" << y << std::endl;
                const T RMS = quark::rms<T>::of(b,y);
                std::cerr << '<' << RMS << '>';
            }
        }
        // std::cerr << std::endl;
    }
}

Y_UTEST(cholesky)
{
    do_test<float>();
    do_test<double>();

}
Y_UTEST_DONE()

