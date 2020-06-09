#include "y/mkl/fcn/bessel.hpp"
#include "y/mkl/fcn/functions.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"
#include "y/string.hpp"

using namespace upsylon;
using namespace math;

namespace {

    template <typename T>
    static inline void do_test()
    {
        {
            const string  fn = vformat("bessel%d.dat",int(sizeof(T))*8);
            ios::ocstream fp(fn);
            for(T x=T(0.01);x<=T(5);x+=T(0.01))
            {
                fp("%g %g %g %g %g\n", x, bessel<T>::j0(x), bessel<T>::y0(x), bessel<T>::i0(x), bessel<T>::k0(x));
            }
        }

        {
            const string fn = vformat("erf%d.dat",int(sizeof(T))*8);
            ios::ocstream fp(fn);
            for(T x=T(-2);x<=T(2);x+=T(0.01))
            {
                const T q  = qerf(x);
                const T qc = qerfc(x);
                const T y  = iqerf(q);
                const T yc = iqerfc(qc);
                fp("%g %g %g %g %g\n", x, qerf(x), qerfc(x), y, yc);
            }
        }

    }
}

Y_UTEST(functions)
{
    do_test<float>();
    do_test<double>();

}
Y_UTEST_DONE()



