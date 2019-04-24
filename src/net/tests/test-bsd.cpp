#include "y/net/bsd-socket.hpp"
#include "y/net/net.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(bsd)
{

    net::bsd_socket tcp_v4(net::tcp,net::v4);
    net::bsd_socket tcp_v6(net::tcp,net::v6);

    net::bsd_socket udp_v4(net::udp,net::v4);
    net::bsd_socket udp_v6(net::udp,net::v6);


}
Y_UTEST_DONE()

