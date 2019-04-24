#include "y/net/net.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

static inline void show_ip( const net::socket_address &ip )
{
    std::cerr << "address=" << ip.className() << std::endl;
    std::cerr << "       |_" << ip.text()     << std::endl;
    std::cerr << "       |_" << net::bswp( ip.port ) << std::endl;
}

Y_UTEST(addr)
{
    const uint16_t p = 8080;
    net::ipv4 ip4( net::ip_addr_none, p );
    net::ipv6 ip6( net::ip_addr_any, p );
    std::cerr << "port4=" << net::bswp( ip4.port ) << std::endl;
    std::cerr << "port6=" << net::bswp( ip6.port ) << std::endl;
    Y_CHECK(p==net::bswp(ip4.port));
    Y_CHECK(p==net::bswp(ip6.port));
    std::cerr << "ip4=" << ip4 << std::endl;
    std::cerr << "ip6=" << ip6 << std::endl;
    ip6._(net::ip_addr_loopback);
    std::cerr << "ip6.addr=" << ip6.addr << std::endl;
    std::cerr << "ip6=" << ip6 << std::endl;

    std::cerr << "sizeof(hrbuff<16>)=" << sizeof(net::hrbuff<16>) << std::endl;
    std::cerr << "sizeof(hrbuff<64>)=" << sizeof(net::hrbuff<64>) << std::endl;
    std::cerr << "sizeof(net::ipv4) =" << sizeof(net::ipv4)       << std::endl;
    std::cerr << "sizeof(net::ipv6) =" << sizeof(net::ipv6)       << std::endl;

    show_ip( ip4 );
    show_ip( ip6 );

}
Y_UTEST_DONE()

