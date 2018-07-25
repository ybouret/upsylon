#include "y/type/point3d.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"

using namespace upsylon;

namespace {

    template <typename T> static inline
    void do_test()
    {
        point2d<T> p, p1( support::get<T>(), support::get<T>() );

        point3d<T> P, P1(support::get<T>(), support::get<T>(), support::get<T>() );
    }

}

Y_UTEST(points)
{
    do_test<float>();
    do_test<double>();
}
Y_UTEST_DONE()

