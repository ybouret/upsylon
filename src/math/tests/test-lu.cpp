#include "support.hpp"
#include "y/math/kernel/lu.hpp"
#include "y/math/kernel/tao.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;
using namespace math;

namespace
{
    template <typename T>
    static inline void do_test(const size_t nmax = 30)
    {
        std::cerr << "Using LU<sizeof(T)=" << sizeof(T) <<">" << std::endl;
        for(size_t n=1;n<=nmax;++n)
        {
            std::cerr << "\t\t" << n << "x" << n << std::endl;
            matrix<T> a(n,n);
            for(size_t i=n;i>0;--i)
            {
                for(size_t j=n;j>0;--j)
                {
                    a[i][j] = support::get<T>();
                }
            }

            const matrix<T> a0 = a;
            if( LU::build(a) )
            {
                std::cerr << "[SUCCESS]" << std::endl;
            }
            else
            {
                std::cerr << "[FAILURE]" << std::endl;
            }

            vector<T> r(n);
            for(size_t i=n;i>0;--i)
            {
                r[i] = support::get<T>();
            }

            vector<T> u = r;
            LU::solve<T>(a,u);
            vector<T> d = r;
            tao::mul_sub(d,a0,u);
            std::cerr << "d=" << d << std::endl;
        }
    }
}

Y_UTEST(LU)
{
    do_test<float>();
    do_test<double>();
    do_test< complex<float>  >();
    do_test< complex<double> >();

    do_test<mpq>(5);

}
Y_UTEST_DONE()

