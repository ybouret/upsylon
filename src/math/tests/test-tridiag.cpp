#include "y/mpl/rational.hpp"
#include "y/math/kernel/tridiag.hpp"
#include "y/utest/run.hpp"
#include <typeinfo>
#include "y/sequence/vector.hpp"
#include "support.hpp"
#include "y/math/kernel/tao.hpp"

using namespace upsylon;
using namespace math;

namespace
{
    template <typename T>
    void do_test()
    {
        std::cerr << "tridiag<" << typeid(T).name() << ">" << std::endl;
        for(size_t n=1;n<=3;++n)
        {
            tridiag<T> t(n);
            vector<T> r(n);
            vector<T> u(n);
            vector<T> v(n);
            const T   xtra = n;
            for(size_t iter=0;iter<16;++iter)
            {
                for(size_t i=1;i<=n;++i)
                {

                    t.a[i] = support::get<T>();
                    t.c[i] = support::get<T>();
                    t.b[i] = xtra+fabs_of(t.a[i])+fabs_of(t.c[i]);
                    r[i]   = support::get<T>();
                }
                //std::cerr << "t=" << t << std::endl;
                //std::cerr << "r=" << r << std::endl;
                t.solve(u,r);
                //std::cerr << "u=" << u << std::endl;
                t.mul(v,u);
                tao::sub(v,r);
                const typename real_for<T>::type d2 = tao::mod2(v);
                std::cerr << "d2=" << d2 << "/";
            }
        }
        std::cerr << std::endl;

    }
}

Y_UTEST(tridiag)
{

    do_test<float>();
    do_test<double>();
    do_test< complex<float>  >();
    do_test< complex<double> >();
    do_test<mpq>();

}
Y_UTEST_DONE()

