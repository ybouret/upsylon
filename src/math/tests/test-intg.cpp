#include "y/math/fcn/integrate.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace math;

namespace {

    template <typename T>
    static inline T f(const T x )
    {
        return square_of(cos_of(x))/(2*numeric<T>::pi);
    }


    template <typename T>
    static inline T g(const T x )
    {
        return sin_of(x*x);
    }

    template <typename T>
    static inline void do_test()
    {
        std::cerr << std::endl << "-- testing with " << sizeof(T)*8 << " bits" << std::endl;
        T sum = 0;
        {
            const T a = T(0);
            const T b = T(2*numeric<T>::pi);
            const T ftol = T(1e-4);
            if(integrate::quad(sum,f<T>,a,b,ftol))
            {
                std::cerr << "success" << std::endl;
            }
            else
            {
                std::cerr << "failure" << std::endl;
            }
            std::cerr << "sum1=" << sum << std::endl;
            sum = integrate::compute(f<T>,a,b,ftol);
            std::cerr << "sum2=" << sum << std::endl;
        }

        {
            const T a = T(0);
            const T b = T(10);
            const T ftol = T(1e-12);
            if(integrate::quad(sum,g<T>,a,b,ftol))
            {
                std::cerr << "success" << std::endl;
            }
            else
            {
                std::cerr << "failure" << std::endl;
            }
            std::cerr << "sum1=" << sum << std::endl;

            sum = integrate::compute(f<T>,a,b,ftol);
            std::cerr << "sum2=" << sum << std::endl;
        }
    }


}

Y_UTEST(intg)
{
    do_test<float>();
    do_test<double>();
}
Y_UTEST_DONE()


