
#include "y/net/io/tcp-server.hpp"

namespace upsylon
{
    namespace net
    {
        tcp_server_protocol:: ~tcp_server_protocol() throw()
        {
        }

        tcp_server_protocol:: tcp_server_protocol( const socket_address &ip, const unsigned pending) :
        tcp_server(ip,pending)
        {
        }


        tcp_client_protocol * tcp_server_protocol:: create(const tcp_link &lnk)
        {
            return new tcp_client_protocol(lnk);
        }
    }
}
