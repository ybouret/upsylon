
#include "y/net/comm/tcp-server.hpp"
#include "y/net/comm/tcp-client.hpp"

namespace upsylon
{
    namespace net
    {
        comm_tcp_server:: ~comm_tcp_server() throw()
        {
        }

        comm_tcp_server:: comm_tcp_server(const socket_address &ip,
                                          const unsigned        pending) :
        tcp_server(ip,pending)
        {
        }

        comm_tcp_client * comm_tcp_server:: create(const tcp_link &link)
        {
            return new comm_tcp_client(link,128,128);
        }
    }

}


