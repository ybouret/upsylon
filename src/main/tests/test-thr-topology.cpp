#include "y/concurrent/device/topology.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(thr_topology)
{
    concurrent::topology topo;

    std::cerr << topo << std::endl;
    for(size_t i=0;i<topo.size;++i)
    {
        std::cerr << i << " -> core#" << topo.core_of(i) << std::endl;
    }

}
Y_UTEST_DONE()


