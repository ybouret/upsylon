#include "y/mpi/mpi.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(init)
{
    Y_MPI(SINGLE);
    if(MPI.isHead)
    {
        std::cerr << "ThreadLevel=" << MPI.threadLevelText() << std::endl;
    }
}
Y_UTEST_DONE()

