

#include "y/mpi++/mpi.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(init_env)
{
    Y_MPI_ENV();
    std::cerr << MPI.threadLevelText() << std::endl;
}
Y_UTEST_DONE()

