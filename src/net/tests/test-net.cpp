#include "y/net/net.hpp"
#include "y/net/types.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(network)
{
    network::verbose  = true;
    network      & nw = network::instance();
    const string   hn = nw.get_host_name();
    std::cerr << "host_name=" << hn << std::endl;

    net128_t x;
    std::cerr << "x=" << x << std::endl;
}
Y_UTEST_DONE()

