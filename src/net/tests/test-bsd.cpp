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

    vector<size_t> h(10001,as_capacity);
    for(int i=0;i<=10000;++i)
    {
        h.push_back( crc32(&i,sizeof(i)) );
    }
    const size_t old_size = h.size();
    unique(h);
    const size_t new_size = h.size();
    std::cerr << "#h: " << old_size << "->" << new_size << std::endl;
}
Y_UTEST_DONE()

