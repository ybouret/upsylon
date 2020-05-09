
#include "y/mpi++/mpi.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(init)
{
    Y_MPI(SINGLE);
    std::cerr << MPI.threadLevelText() << std::endl;
    std::cerr << MPI.processorName << std::endl;
    std::cerr << MPI.nodeName      << std::endl;
}
Y_UTEST_DONE()

