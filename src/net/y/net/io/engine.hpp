//! \file
#ifndef Y_NET_IO_ENGINE_INCLUDED
#define Y_NET_IO_ENGINE_INCLUDED 1

#include "y/net/socket/set.hpp"
#include "y/net/io/tcp-server.hpp"
#include "y/net/io/tcp-client.hpp"


namespace upsylon
{
    namespace net
    {

      

        //! handle connections
        class engine : public net_object
        {
        public:

            explicit engine();         //!< setup
            virtual ~engine() throw(); //!< cleanup

            void start( tcp_server_protocol *srv ); //!< register a new tcp protocol

            bool cycle( socket_delay &d ); //!< probe incoming/outgoing

        private:
            Y_DISABLE_COPY_AND_ASSIGN(engine);
            socket_set    sockset;
            tcp_server_db tcp_servers;
            tcp_client_db tcp_clients;

            bool handle_tcp_servers(size_t &na);
            bool handle_tcp_clients_incoming(size_t &na);
        };
    }
}

#endif

