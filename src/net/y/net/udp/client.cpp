
#include "y/net/udp/client.hpp"

namespace upsylon
{
    namespace net
    {
        udp_client:: ~udp_client() throw()
        {
        }

        udp_client:: udp_client( const socket_address &ip ) :
        socket_addr_ex(ip),
        udp_socket( (**this).version() )
        {
            
        }

        void udp_client::send(const void *data, const size_t size, const int flags) const
        {
            sendto( **this, data, size, flags);
        }


    }
}

