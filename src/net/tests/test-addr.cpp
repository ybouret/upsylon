#include "y/net/net.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;

static inline void show_ip( const net::socket_address &ip )
{
    std::cerr << "address=" << ip.className() << std::endl;
    std::cerr << "       |_" << ip.text()     << std::endl;
    std::cerr << "       |_" << net::bswp( ip.port ) << std::endl;
}

#define SHOW(TYPE) do {\
std::cerr.flush();\
ios::ocstream fp( ios::cstderr ); \
const string s = "sizeof(" #TYPE ")";\
fp.align(s,28,' ')(" = %u\n",unsigned(sizeof(TYPE)));\
fp.flush();\
} while(false)


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

    SHOW(net::hrbuff<16>);
    SHOW(net::hrbuff<64>);
    SHOW(net::ipv4);
    SHOW(net::ipv6);

    show_ip( ip4 );
    show_ip( ip6 );

    net::socket_addr_ex h4(ip4);
    net::socket_addr_ex h6(ip6);
    std::cerr << "hook4: " << *h4   << std::endl;
    std::cerr << "hook6: " << *h6   << std::endl;

}
Y_UTEST_DONE()

