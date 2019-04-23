#include "y/net/ip-socket.hpp"
#include "y/net/net.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(bsd)
{

    net::ip_socket tcp_v4(net::tcp,net::v4);
    net::ip_socket tcp_v6(net::tcp,net::v6);

    net::ip_socket udp_v4(net::udp,net::v4);
    net::ip_socket udp_v6(net::udp,net::v6);


}
Y_UTEST_DONE()

