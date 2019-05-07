#include "y/net/socket/bsd.hpp"
#include "y/net/net.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/sort/unique.hpp"

using namespace upsylon;

static inline void display( net::bsd_socket &s )
{

    std::cerr << "\t<SOCKET>" << std::endl;
    const int sb = s.sndbuf();
    const int rb = s.rcvbuf();
    std::cerr << "\tsocket: sndbuf=" << sb << " | rcvbuf=" << rb << std::endl;
    s.blocking(true);
    s.blocking(false);
    std::cerr << "\t<SOCKET/>" << std::endl << std::endl;
}


Y_UTEST(bsd)
{
    network::verbose = true;
    net::bsd_socket tcp_v4(net::tcp,net::v4); display(tcp_v4);
    net::bsd_socket tcp_v6(net::tcp,net::v6); display(tcp_v6);
    net::bsd_socket udp_v4(net::udp,net::v4); display(udp_v4);
    net::bsd_socket udp_v6(net::udp,net::v6); display(udp_v6);

    std::cerr << "sizeof(bsd_socket)   =" << sizeof(net::bsd_socket)  << std::endl;
    std::cerr << "sizeof(socket_type)  =" << sizeof(net::socket_type) << std::endl;
    std::cerr << "sizeof(socket_id_t)  =" << sizeof(net::socket_id_t) << std::endl;
    std::cerr << "bsd_socket.uid_bytes =" << net::bsd_socket::uid_bytes << std::endl;
    std::cerr << "bsd_socket.rnd_bytes =" << net::bsd_socket::rnd_bytes   << std::endl;

    std::cerr << std::endl;

}
Y_UTEST_DONE()

