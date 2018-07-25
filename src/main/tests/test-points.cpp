#include "y/type/point3d.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"

using namespace upsylon;

namespace {

    template <typename T> static inline
    void do_test()
    {
        point2d<T> p, p1( support::get<T>(), support::get<T>() );
        p = +p1;
        p = p+p1;
        p += p1;

        p = -p1;
        p -= p1;
        p  = p-p1;

        p *= support::get<T>();
        p  = p1 * support::get<T>();
        p  = support::get<T>() * p1;
        std::cerr << "dot="   << p*p1 << std::endl;
        std::cerr << "norm2=" << p.norm2() << std::endl;

        point3d<T> P, P1(support::get<T>(), support::get<T>(), support::get<T>() );
        P = +P1;
        P = P+P1;
        P += P1;

        P = -P1;
        P -= P1;
        P  = P - P1;

        P *= support::get<T>();
        P  = P1 * support::get<T>();
        P  = support::get<T>() * P1;
        std::cerr << "dot="   << P*P1 << std::endl;
        std::cerr << "norm2=" << P.norm2() << std::endl;
    }

}

Y_UTEST(points)
{
    do_test<float>();
    do_test<double>();
}
Y_UTEST_DONE()

