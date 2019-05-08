#include "y/net/io/engine.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(io)
{
    network::verbose = true;
    net::engine E;

    {
        const net::ipv4 server_ip( net::ip_addr_any, 12000 );
        E.start( new net::tcp_server_protocol(server_ip) );
    }


    net::socket_delay d = 5;
    E.cycle(d);


}
Y_UTEST_DONE()


