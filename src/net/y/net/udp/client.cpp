
#include "y/net/udp/client.hpp"

namespace upsylon
{
    namespace net
    {
        udp_client:: ~udp_client() throw()
        {
            Y_NET_VERBOSE(std::cerr << "[network.udp_client.quit]" << std::endl);
        }

        static inline void __udp_client_init()
        {
            Y_NET_VERBOSE(std::cerr << "[network.udp_client.init]" << std::endl);
        }

#define Y_NET_UDP_CLIENT( INI ) socket_addr_ex INI,  udp_socket( (**this).version() ), last_recv_ip(ip_addr_none, (**this).version(),0 )

        udp_client:: udp_client(const socket_address &ip ) :                     Y_NET_UDP_CLIENT( (ip) )            { __udp_client_init(); }
        udp_client:: udp_client(const string &xname, const ip_version version) : Y_NET_UDP_CLIENT( (xname,version) ) { __udp_client_init(); }
        udp_client:: udp_client(const char   *xname, const ip_version version) : Y_NET_UDP_CLIENT( (xname,version) ) { __udp_client_init(); }

        void udp_client:: send_(const void *data, const size_t size) const
        {
            sendto( **this, data, size );
        }

        
        size_t udp_client:: recv_(void *data, const size_t size)
        {
            return recvfrom(*last_recv_ip,data,size);
        }

    }
}

