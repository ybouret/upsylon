
#include "y/net/udp/server.hpp"


namespace upsylon
{
    namespace net
    {
        udp_server:: ~udp_server() throw()
        {
            Y_NET_VERBOSE(std::cerr << "[network.udp_server.quit]" << std::endl);
        }

#define Y_NET_UDP_SERVER(INI)           \
socket_addr_ex INI,                     \
udp_socket( (**this).version() ),       \
peer(ip_addr_none, (**this).version() )

        static inline void __udp_server_init()
        {
            Y_NET_VERBOSE(std::cerr << "[network.udp_server.init]" << std::endl);
        }

        udp_server:: udp_server( const socket_address &ip ) :
        Y_NET_UDP_SERVER( (ip) )
        {
            __udp_server_init();
            (**this).bind(sock);
        }

        udp_server:: udp_server( const uint16_t user_port, const ip_version version ) :
        Y_NET_UDP_SERVER( (ip_addr_any,version,user_port) )
        {
            __udp_server_init();
            (**this).bind(sock);
        }
        
        size_t udp_server:: recv_(void *data, const size_t size)
        {
            return recvfrom(*peer,data,size);
        }

        void udp_server:: send_( const void *data, const size_t size) const
        {
            sendto(*peer,data,size);
        }

    }

}
