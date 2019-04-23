#include "y/net/bsd.hpp"
#include "y/net/net.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(bsd)
{
    const string host_name = network::instance().get_host_name();

#if 0
    {
        net::bsd::socket_type s = net::bsd::acquire_socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
        net::bsd::release_socket(s);
    }

    {
        net::bsd::socket_type s = net::bsd::acquire_socket(PF_INET6,SOCK_STREAM,IPPROTO_TCP);
        net::bsd::release_socket(s);
    }

    {
        net::bsd::socket_type s = net::bsd::acquire_socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
        net::bsd::release_socket(s);
    }

    {
        net::bsd::socket_type s = net::bsd::acquire_socket(PF_INET6,SOCK_DGRAM,IPPROTO_UDP);
        net::bsd::release_socket(s);
    }
#endif
    

}
Y_UTEST_DONE()

