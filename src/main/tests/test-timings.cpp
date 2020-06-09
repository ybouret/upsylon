#include "y/mkl/timings.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(timings)
{
    double t_run = 1.1;
    double dt    = 0.012;
    double save  = 0.05;
    size_t every = 0;

    std::cerr << "t_run=" << t_run << ", dt=" << dt << ", save=" << save << std::endl;
    const size_t iters = math::timings::setup(t_run, dt, save, every);
    std::cerr << "iters=" << iters << ", t_run=" << t_run << ", save=" << save << ", every=" << every << ", dt=" << dt << std::endl;
}
Y_UTEST_DONE()



