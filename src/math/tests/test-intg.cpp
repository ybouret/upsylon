#include "y/math/fcn/integrate.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace math;

namespace {

    template <typename T>
    static inline T f(const T x )
    {
        return square_of(__cos(x))/(2*numeric<T>::pi);
    }

    template <typename T>
    static inline void do_test()
    {
        T sum = 0;
        if(integrate::quad(sum,f<T>,T(0),T(2*numeric<T>::pi),T(1e-12)))
        {
            std::cerr << "success" << std::endl;
        }
        else
        {
            std::cerr << "failure" << std::endl;
        }
        std::cerr << "sum=" << sum << std::endl;
    }


}

Y_UTEST(intg)
{
    do_test<float>();
    do_test<double>();
}
Y_UTEST_DONE()


