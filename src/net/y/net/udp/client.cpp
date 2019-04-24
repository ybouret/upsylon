
#include "y/net/udp/client.hpp"

namespace upsylon
{
    namespace net
    {
        udp_client:: ~udp_client() throw()
        {
        }

#define Y_NET_UDP_CLIENT( INI ) socket_addr_ex INI,  udp_socket( (**this).version() ), last_recv_ip(ip_addr_none, (**this).version(),0 )

        udp_client:: udp_client(const socket_address &ip ) : Y_NET_UDP_CLIENT( (ip) ) {}
        udp_client:: udp_client(const string &xname, const ip_version version) : Y_NET_UDP_CLIENT( (xname,version) ) {}
        udp_client:: udp_client(const char   *xname, const ip_version version) : Y_NET_UDP_CLIENT( (xname,version) ) {}

        void udp_client:: send_block(const void *data, const size_t size) const
        {
            sendto( **this, data, size );
        }

        
        size_t udp_client:: recv(void *data, const size_t size)
        {
            return recvfrom(*last_recv_ip,data,size);
        }

    }
}

