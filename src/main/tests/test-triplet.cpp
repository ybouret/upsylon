#include "y/mkl/triplet.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"

using namespace upsylon;
using namespace mkl;

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

        for(size_t i=1;i<=3;++i)
        {
            std::cerr << "x[" << i << "]=" << x[i] << std::endl;
        }

        std::cerr << "size=" << x.size() << std::endl;

        triplet<T> u;
        u.ld(x);
        std::cerr << "x=" << x << std::endl;
        std::cerr << "u=" << u << std::endl;
        Y_CHECK(x.diff2(u)<=0);
        for(size_t i=1;i<=3;++i)
        {
            u[i] = support::get<T>();
        }
        x.ld(u);
        std::cerr << "x=" << x << std::endl;
        std::cerr << "u=" << u << std::endl;
        Y_CHECK(x.diff2(u)<=0);

    }
}

Y_UTEST(triplet)
{
    do_test<int>();
    do_test<float>();
    do_test<double>();

}
Y_UTEST_DONE()
