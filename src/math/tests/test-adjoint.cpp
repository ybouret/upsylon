#include "support.hpp"
#include "y/math/kernel/adjoint.hpp"
#include "y/math/kernel/tao.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace math;

namespace {

    template <typename T>
    static inline void do_test()
    {
        for(size_t n=1;n<=4;++n)
        {
            matrix<T> a(n,n);
            for(size_t i=1;i<=n;++i)
            {
                for(size_t j=1;j<=n;++j)
                {
                    a[i][j] = support::get<T>();
                }
            }
            const T det = determinant(a);
            std::cerr << "a="  << a << std::endl;
            std::cerr << "det=" << det << std::endl;
            matrix<T> b(n,n);
            adjoint(b,a);
            std::cerr << "b=" << b << std::endl;
            matrix<T> p(n,n);
            tao::mmul(p,a,b);
            std::cerr << "p=" << p << std::endl;
        }
    }
}

Y_UTEST(adjoint)
{
    do_test< complex<double> >();
    do_test<float>();
    do_test<double>();
    do_test<mpq>();

    {
        const size_t  n = 4;
        matrix<short> U(n,n);
        for(size_t i=1;i<=n;++i) for(size_t j=1;j<=n;++j) U[i][j] = alea.range<short>(-100,100);
        std::cerr << "U=" << U << std::endl;
        matrix<int64_t>   A(n,n);
        iadjoint(A,U);
        std::cerr << "A=" << A << std::endl;
        matrix<int64_t> P(n,n);
        tao::mmul(P,A,U);
        std::cerr << "P=" << P << std::endl;
    }

}
Y_UTEST_DONE()

