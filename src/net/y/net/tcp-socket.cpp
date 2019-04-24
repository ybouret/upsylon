#include "y/net/tcp-socket.hpp"

namespace upsylon
{
    namespace net
    {

        tcp_socket:: ~tcp_socket() throw()
        {
        }

        tcp_socket:: tcp_socket( const ip_version version ) :
        bsd_socket(tcp,version)
        {
        }

        

    }
}

