//! \file
#ifndef Y_NET_UDP_SOCKET_INCLUDED
#define Y_NET_UDP_SOCKET_INCLUDED 1

#include "y/net/ip-socket.hpp"

namespace upsylon
{
    namespace net
    {

        class udp_socket : public ip_socket
        {
        public:
            virtual ~udp_socket() throw();

        protected:
            explicit udp_socket( const ip_version version);


        private:
            Y_DISABLE_COPY_AND_ASSIGN(udp_socket);
        };

    }
}

#endif

