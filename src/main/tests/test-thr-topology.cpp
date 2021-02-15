#include "y/concurrent/device/topology.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(thr_topology)
{
    concurrent::topology topo;

    std::cerr << topo << std::endl;

}
Y_UTEST_DONE()


