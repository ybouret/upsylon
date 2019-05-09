#include "y/net/tcp/server.hpp"
#include "y/net/tcp/stream.hpp"
#include "y/net/udp/server.hpp"
#include "y/net/udp/client.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace net;

#define SHOW(TYPE) do {\
ios::ocstream fp( ios::cstderr ); \
const string s = "sizeof(" #TYPE ")";\
fp.align(s,32,' ')(" = %u\n",unsigned(sizeof(TYPE)));\
} while(false)

Y_UTEST(info)
{

    SHOW(bsd_socket);
    SHOW(socket_address);
    SHOW(ipv4);
    SHOW(ipv6);
    SHOW(socket_addr_ex);
    SHOW(udp_server);
    SHOW(udp_client);
    SHOW(tcp_server);
    SHOW(tcp_client);
    SHOW(tcp_ostream);
    SHOW(tcp_istream);
    SHOW(tcp_cache);
    SHOW(byte_node);

}
Y_UTEST_DONE()


