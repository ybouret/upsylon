//! \file
#ifndef Y_NET_UDP_CLIENT_INCLUDED
#define Y_NET_UDP_CLIENT_INCLUDED 1

#include "y/net/udp/socket.hpp"

namespace upsylon
{
    namespace net
    {
        class udp_client :  public socket_addr_ex, public udp_socket
        {
        public:
            virtual ~udp_client() throw();
            explicit udp_client(const socket_address &ip);

            void send( const void *data, const size_t size, const int flags=0 ) const;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(udp_client);
        };
    }
}

#endif

