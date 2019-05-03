#include "y/net/socket/bsd.hpp"
#include "y/net/socket/hasher.hpp"
#include "y/net/net.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

static inline void display( net::bsd_socket &s )
{
    net::socket_hasher H;

    std::cerr << "\t<SOCKET>" << std::endl;
    std::cerr << "\tH=" << H(s.key()) << std::endl;
    std::cerr << "\tsocket: sndbuf=" << s.sndbuf() << " | rcvbuf=" << s.rcvbuf() << std::endl;
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
    
#if 0
    uint32_t s_odd = 0;
    for(unsigned i=0;i<=15;++i)
    {
        unsigned p=(i<<1)+1;
        s_odd += (1<<p);
    }
    std::cerr << std::hex << "0x" << s_odd << std::dec << std::endl;
    uint32_t s_evn = s_odd>>1;
    std::cerr << std::hex << "0x" << s_evn << std::dec << std::endl;
    uint32_t s_all = s_evn + s_odd;
    std::cerr << std::hex << "0x" << s_all << std::dec << std::endl;
#endif
    
}
Y_UTEST_DONE()

