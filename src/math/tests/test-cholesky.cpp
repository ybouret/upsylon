#include "y/math/kernel/cholesky.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/math/kernel/atom.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;
using namespace math;

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
                atom::mmul_rtrn(A, P, P);
                A0 = A;
                //std::cerr << "P =" << P  << std::endl;
                //std::cerr << "A0=" << A0 << std::endl;

                if( !cholesky::build(A) ) continue;
                //std::cerr << "A =" << A  << std::endl;


                for(size_t i=1;i<=n;++i)
                {
                    b[i] = support::get<T>() - T(0.5);
                }
                //A*x=b
                cholesky::solve(x, A, b);
                atom::mul(y, A0, x);
                //std::cerr << "b=" << b << ":y=" << y << std::endl;
                const T RMS = atom::rms(b,y);
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

