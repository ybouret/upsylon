#include "y/math/triplet.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"

using namespace upsylon;
using namespace math;

namespace
{
    template <typename T>
    static inline
    void do_test()
    {
        triplet<T> x = { support::get<T>(), support::get<T>(), support::get<T>() };
        triplet<T> y = { support::get<T>(), support::get<T>(), support::get<T>() };
        std::cerr << "x_raw=" << x << std::endl;
        x.sort();
        std::cerr << "x_srt=" << x << std::endl;
        std::cerr << "y_raw=" << y << std::endl;
        y.co_sort(x);
        std::cerr << "y_co_srt=" << y << std::endl;
        std::cerr << "x_co_srt=" << x << std::endl;
    }
}

Y_UTEST(triplet)
{
    do_test<int>();
    do_test<float>();
    do_test<double>();

}
Y_UTEST_DONE()
