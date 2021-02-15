#include "y/concurrent/device/topology.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace
{
   
}

Y_UTEST(thr_topology)
{
    concurrent::topology topo;

    std::cerr << topo << std::endl;
    for(const concurrent::topology::node *node=topo.nodes.head;node;node=node->next)
    {
        std::cerr << "-> " << node->rank << std::endl;
    }
    
    std::cerr << "sizeof(topology)=" << sizeof(concurrent::topology) << std::endl;
}
Y_UTEST_DONE()


