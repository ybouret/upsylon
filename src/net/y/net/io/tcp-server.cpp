
#include "y/net/io/tcp-server.hpp"

namespace upsylon
{
    namespace net
    {
        tcp_server_protocol:: ~tcp_server_protocol() throw()
        {
        }

        tcp_server_protocol:: tcp_server_protocol( const socket_address &ip ) :
        tcp_server(ip,1)
        {
        }
        
    }
}



#include "y/net/io/tcp-client.hpp"

namespace upsylon
{
    namespace net
    {
        tcp_client_protocol * tcp_server_protocol:: create(const tcp_link &link)
        {

            return new tcp_client_protocol( link );
        }
    }

}
