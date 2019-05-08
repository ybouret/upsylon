//! \file
#ifndef Y_NET_IO_ENGINE_INCLUDED
#define Y_NET_IO_ENGINE_INCLUDED 1

#include "y/net/socket/set.hpp"
#include "y/net/tcp/server.hpp"

namespace upsylon
{
    namespace net
    {

        class tcp_client_protocol;

        class tcp_server_protocol : public tcp_server
        {
        public:
            virtual ~tcp_server_protocol() throw() {}

            explicit tcp_server_protocol();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(tcp_server_protocol);
        };


        //! handle connections
        class engine : public net_object
        {
        public:

            explicit engine();         //!< setup
            virtual ~engine() throw(); //!< cleanup


        private:
            Y_DISABLE_COPY_AND_ASSIGN(engine);
            socket_set sockset;
            
            
        };
    }
}

#endif

