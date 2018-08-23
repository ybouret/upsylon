#include "y/math/ode/solver-data.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace math;

namespace {

    template <typename T>
    static inline void do_test()
    {
        ODE::SolverData<T> sd;
        std::cerr << "solver_data.eps=" << sd.eps << std::endl;
    }
}

Y_UTEST(ode)
{
    do_test<float>();
    do_test<double>();
}
Y_UTEST_DONE()

