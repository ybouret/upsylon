
#include "y/net/udp-socket.hpp"

namespace upsylon
{
    namespace net
    {

        udp_socket:: ~udp_socket() throw()
        {
        }

        udp_socket:: udp_socket( const ip_version version ) :
        ip_socket(udp,version)
        {
        }



    }
}

