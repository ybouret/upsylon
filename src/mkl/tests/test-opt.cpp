
#include "y/mkl/opt/optimize.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace mkl;

namespace
{
    template <typename T> inline
    T F(T x)
    {
        return cos_of(x*x*T(0.3));
    }


    template <typename T> inline
    void doOpt()
    {
        const T x0 = 4*alea.to<T>();
        const T x1 = 4*alea.to<T>();
        const T xm = optimize(F<T>,x0,x1);

        std::cerr << "xm=" << xm << std::endl;

    }

}


Y_UTEST(opt)
{
    doOpt<float>();
    doOpt<double>();
}
Y_UTEST_DONE()

