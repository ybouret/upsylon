#include "y/net/tcp/socket.hpp"

namespace upsylon
{
    namespace net
    {

        tcp_socket:: ~tcp_socket() throw()
        {
            Y_NET_VERBOSE("[network.tcp_socket.quit]");
        }

        tcp_socket:: tcp_socket( const ip_version version ) :
        bsd_socket(tcp,version)
        {
            Y_NET_VERBOSE("[network.tcp_socket.init]");
        }

        tcp_socket:: tcp_socket( const socket_type &s) :
        bsd_socket(s)
        {
        }
        

    }
}

