//! \file
#ifndef Y_NET_TCP_SERVER_INCLUDED
#define Y_NET_TCP_SERVER_INCLUDED 1

#include "y/net/tcp/socket.hpp"

namespace upsylon
{
    namespace net
    {

        //! TCP server
        class tcp_server : public tcp_socket
        {
        public:
            virtual ~tcp_server() throw(); //! destructor
            explicit tcp_server();


        private:
            Y_DISABLE_COPY_AND_ASSIGN(tcp_server);
        };

    }
}

#endif
