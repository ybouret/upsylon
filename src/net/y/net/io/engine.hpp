//! \file
#ifndef Y_NET_IO_ENGINE_INCLUDED
#define Y_NET_IO_ENGINE_INCLUDED 1

#include "y/net/socket/set.hpp"
#include "y/net/io/tcp-server.hpp"


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

            void start( tcp_server_protocol *srv );

            bool cycle( socket_delay &d );

        private:
            Y_DISABLE_COPY_AND_ASSIGN(engine);
            socket_set    sockset;
            tcp_server_db tcp_servers;
            
        };
    }
}

#endif

