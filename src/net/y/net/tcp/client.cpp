
#include "y/net/tcp/client.hpp"

namespace upsylon
{
    namespace net
    {

        tcp_client:: ~tcp_client() throw()
        {
            Y_NET_VERBOSE(std::cerr << "[network.tcp_client.quit]" << std::endl);
        }

#define Y_NET_TCP_CLIENT( INI ) socket_addr_ex INI,  tcp_socket( (**this).version() )

        tcp_client:: tcp_client(const socket_address &ip ) :                     Y_NET_TCP_CLIENT( (ip) )            { start(); }
        tcp_client:: tcp_client(const string &xname, const ip_version version) : Y_NET_TCP_CLIENT( (xname,version) ) { start(); }
        tcp_client:: tcp_client(const char   *xname, const ip_version version) : Y_NET_TCP_CLIENT( (xname,version) ) { start(); }

        void tcp_client:: start()
        {
            Y_NET_VERBOSE(std::cerr << "[network.tcp_client.init]" << std::endl);
            (*this)->connect(sock);
        }

    }

}

