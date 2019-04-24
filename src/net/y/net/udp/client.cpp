
#include "y/net/udp/client.hpp"

namespace upsylon
{
    namespace net
    {
        udp_client:: ~udp_client() throw()
        {
        }

        udp_client:: udp_client( const socket_address &dest ) :
        socket_hook(dest),
        udp_socket( (**this).version() )
        {
        }
        
    }
}

