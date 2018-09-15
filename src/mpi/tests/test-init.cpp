#include "y/mpi/mpi.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(init)
{
    mpi & MPI = mpi::instance();
}
Y_UTEST_DONE()

