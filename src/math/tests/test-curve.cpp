#include "y/math/signal/curve.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace math;


template <typename T>
static inline void show_info()
{
    std::cerr << "sizeof=" << sizeof(T) << std::endl;
    std::cerr << "\ttype1d: " <<  curve::info_for<T>::dim << std::endl;
    std::cerr << "\ttype2d: " <<  curve::info_for< point2d<T> >::dim << std::endl;
    std::cerr << "\ttype3d: " <<  curve::info_for< point3d<T> >::dim << std::endl;
    Y_ASSERT(sizeof(T)==sizeof(typename curve::info_for<T>::real));
}

Y_UTEST(curve)
{
    show_info<float>();
    show_info<double>();

}
Y_UTEST_DONE()

