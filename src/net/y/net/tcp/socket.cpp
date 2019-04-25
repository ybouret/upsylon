#include "y/net/tcp/socket.hpp"

namespace upsylon
{
    namespace net
    {

        tcp_socket:: ~tcp_socket() throw()
        {
            Y_NET_VERBOSE(std::cerr << "[network.tcp_socket.quit]" << std::endl);
        }

        tcp_socket:: tcp_socket( const ip_version version ) :
        bsd_socket(tcp,version)
        {
            Y_NET_VERBOSE(std::cerr << "[network.tcp_socket.init]" << std::endl);
        }

        tcp_socket:: tcp_socket( const socket_type &s) :
        bsd_socket(s)
        {
        }
        

    }
}

