
//! \file

#ifndef Y_NET_COMM_ENGINE_INCLUDED
#define Y_NET_COMM_ENGINE_INCLUDED 1

#include "y/net/comm/tcp-server.hpp"
#include "y/net/socket/set.hpp"

namespace upsylon
{
    namespace net
    {

        class comm_engine
        {
        public:
            virtual ~comm_engine() throw();
            explicit comm_engine() throw();



        private:
            socket_set sockset;
            Y_DISABLE_COPY_AND_ASSIGN(comm_engine);
        };

    }

}

#endif
