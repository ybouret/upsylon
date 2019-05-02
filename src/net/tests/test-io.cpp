#include "y/net/io/engine.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(io)
{
    network::verbose = true;
    net::io_engine engine;

    {
        net::ipv4 ip( net::ip_addr_any, 12000 );
        engine.start( new net::tcp_server_protocol(ip) );
    }

}
Y_UTEST_DONE()


