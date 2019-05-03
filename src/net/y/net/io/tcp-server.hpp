//! \file

#ifndef Y_NET_IO_TCP_SERVER_INCLUDED
#define Y_NET_IO_TCP_SERVER_INCLUDED 1

#include "y/net/io/tcp-client.hpp"
#include "y/net/tcp/server.hpp"
#include "y/ptr/intr.hpp"

namespace upsylon
{
    namespace net
    {
        //! handle tcp server connection
        class tcp_server_protocol : public tcp_server
        {
        public:
            explicit tcp_server_protocol(const socket_address &ip,  //|
                                         const unsigned pending=1); //!< setup
            virtual ~tcp_server_protocol() throw();                 //!< cleanup

            virtual  tcp_client_protocol * create( const tcp_link & ); //!< create a matching protocol

        private:
            Y_DISABLE_COPY_AND_ASSIGN(tcp_server_protocol);
        };

        //! dynamic tcp_server_protocol
        typedef intr_ptr<size_t,tcp_server_protocol> tcp_server_proto;

    }
}

#endif


