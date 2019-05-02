#include "y/net/socket/bsd.hpp"
#include "y/net/net.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

static inline void display( net::bsd_socket &s )
{
    std::cerr << "socket: sndbuf=" << s.sndbuf() << " | rcvbuf=" << s.rcvbuf() << std::endl;
    s.blocking(true);
    s.blocking(false);
}


Y_UTEST(bsd)
{
    network::verbose = true;
    net::bsd_socket tcp_v4(net::tcp,net::v4); display(tcp_v4);
    net::bsd_socket tcp_v6(net::tcp,net::v6); display(tcp_v6);
    net::bsd_socket udp_v4(net::udp,net::v4); display(udp_v4);
    net::bsd_socket udp_v6(net::udp,net::v6); display(udp_v6);

    const uint16_t w = 0x1234;
    union
    {
        uint32_t dw;
        uint16_t w;
    } tmp = { 0 };
    tmp.w = swap_le(w);
    const uint32_t dw = swap_le(tmp.dw);
    std::cerr << std::hex << "w=" << w << ", dw=" << dw << std::dec << std::endl;

}
Y_UTEST_DONE()

