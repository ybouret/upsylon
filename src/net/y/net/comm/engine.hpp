
//! \file

#ifndef Y_NET_COMM_ENGINE_INCLUDED
#define Y_NET_COMM_ENGINE_INCLUDED 1

#include "y/net/comm/tcp-server.hpp"
#include "y/net/socket/set.hpp"

namespace upsylon
{
    namespace net
    {
        //______________________________________________________________________
        //
        //
        //! engine for async comms
        //
        //______________________________________________________________________
        class comm_engine
        {
        public:
            virtual ~comm_engine() throw(); //!< cleanup
            explicit comm_engine();         //!< setup
            
            void start( comm_tcp_server *srv );

        private:
            Y_DISABLE_COPY_AND_ASSIGN(comm_engine);
            socket_set          sockset;
            comm_tcp_server::db tcp_servers;
            
        };

    }

}

#endif
