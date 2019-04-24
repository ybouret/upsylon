//! \file
#ifndef Y_NET_TCP_SOCKET_INCLUDED
#define Y_NET_TCP_SOCKET_INCLUDED 1

#include "y/net/bsd-socket.hpp"

namespace upsylon
{
    namespace net
    {

        class tcp_socket : public bsd_socket
        {
        public:
            virtual ~tcp_socket() throw();

        protected:
            explicit tcp_socket( const ip_version version);
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(tcp_socket);
        };

    }
}

#endif

