
//! \file
#ifndef Y_NET_UDP_SERVER_INCLUDED
#define Y_NET_UDP_SERVER_INCLUDED 1

#include "y/net/udp/socket.hpp"

namespace upsylon
{
    namespace net
    {

        class udp_server : public socket_addr_ex, public udp_socket
        {
        public:
            virtual ~udp_server() throw();
            explicit udp_server( const socket_address &ip );
            explicit udp_server( const uint16_t user_port, const ip_version);
            
            socket_addr_ex peer;

            virtual size_t recv( void *data,       const size_t size);
            virtual void   send_block( const void *data, const size_t size) const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(udp_server);
        };

    }

}

#endif


