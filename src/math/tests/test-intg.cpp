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

    // intg(0->10) : 0.5836707004343648
    template <typename T>
    static inline T g(const T x )
    {
        return sin_of(T(0.2)*x*x);
    }



    static inline double upper_y( double x )
    {
        if(x<=-1)
        {
            return 0;
        }
        else if(x>=1)
        {
            return 0;
        }
        else
            return sqrt(1.0-x*x);
    }

    static inline double lower_y( double x )
    {
        return -upper_y(x);
    }

    static inline double just1(double,double)
    {
        return 1.0;
    }

    static inline double radius(double x,double y)
    {
        return sqrt(x*x+y*y);
    }


    template <typename T>
    static inline void do_test()
    {
        std::cerr << std::endl << "-- testing with " << sizeof(T)*8 << " bits" << std::endl;

        T sum = 0;
        {
            std::cerr << " |_f: 0.5" << std::endl;
            const T a    = T(0);
            const T b    = T(2*numeric<T>::pi);
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
            size_t  count = 0;
            sum = integrate::compute(f<T>,a,b,ftol,&count);
            std::cerr << "sum2=" << sum << ", count=" << count << std::endl;
        }

        {
            std::cerr << " |_g:  " << std::endl;
            const T a    = T(0);
            const T b    = T(4);
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

            size_t  count = 0;
            sum = integrate::compute(g<T>,a,b,ftol,&count);
            std::cerr << "sum2=" << sum << ", count=" << count << std::endl;
        }
    }


}

Y_UTEST(intg)
{
    do_test<float>();
    do_test<double>();
    return 0;
    
    std::cerr << "multi-dimensional integral" << std::endl;
    const double area = integrate::compute2(just1,-1.0,1.0,lower_y,upper_y,0.8*1e-5);
    std::cerr << "area=" << area << "/" << numeric<double>::pi << std::endl;

    //return 0;
    const double mu1 = integrate::compute2(radius,-1.0,1.0,lower_y,upper_y,0.8*1e-5);
    std::cerr << "mu1=" << mu1 << "/" << (2*numeric<double>::pi/3) << std::endl;

}
Y_UTEST_DONE()


