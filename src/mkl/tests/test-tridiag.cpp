#include "y/mpl/rational.hpp"
#include "y/math/kernel/tridiag.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "support.hpp"
#include "y/math/kernel/quark.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;
using namespace math;

namespace
{
    template <typename T>
    void do_test()
    {
        std::cerr << "tridiag<" << type_name_of<T>() << ">" << std::endl;
		T xtra = 0;
        for(size_t n=1;n<=3;++n)
        {
            tridiag<T> t(n);
            vector<T>  r(n);
            vector<T>  u(n);
            vector<T>  v(n);
			xtra += T(1);
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
                quark::sub(v,r);
                const typename real_for<T>::type d2 = quark::mod2<T>::of(v);
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

