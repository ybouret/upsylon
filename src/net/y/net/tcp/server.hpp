//! \file
#ifndef Y_NET_TCP_SERVER_INCLUDED
#define Y_NET_TCP_SERVER_INCLUDED 1

#include "y/net/tcp/client.hpp"

namespace upsylon
{
    namespace net
    {


        //! TCP server
        class tcp_server : public socket_addr_ex, public tcp_socket
        {
        public:
            virtual ~tcp_server() throw();                                                                   //! destructor
            explicit tcp_server( const socket_address &ip, const unsigned pending=1);                        //!< bind to specific address
            explicit tcp_server( const uint16_t user_port, const unsigned pending=1, const ip_version = v4); //!< bind to ANY and port

            tcp_client * accept() const; //!< accept a new connection

        private:
            Y_DISABLE_COPY_AND_ASSIGN(tcp_server);
            void start(const unsigned pending);
            socket_type __accept( socket_address &ip ) const;
        };

    }
}

#endif
