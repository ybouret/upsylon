
#include "y/net/udp/client.hpp"

namespace upsylon
{
    namespace net
    {
        udp_client:: ~udp_client() throw()
        {
        }

#define Y_NET_UDP_CLIENT( INI ) socket_addr_ex INI,  udp_socket( (**this).version() )

        udp_client:: udp_client(const socket_address &ip ) : Y_NET_UDP_CLIENT( (ip) ) {}
        udp_client:: udp_client(const string &xname, const ip_version version) : Y_NET_UDP_CLIENT( (xname,version) ) {}
        udp_client:: udp_client(const char   *xname, const ip_version version) : Y_NET_UDP_CLIENT( (xname,version) ) {}

        void udp_client::send(const void *data, const size_t size) const
        {
            sendto( **this, data, size );
        }

        void udp_client:: send(const char *data) const
        {
            send( data, length_of(data) );
        }

        void udp_client:: send( const memory::ro_buffer &buff ) const
        {
            send( buff.ro(), buff.length() );
        }

    }
}

