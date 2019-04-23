//! \file
#ifndef Y_NET_IP_SOCKET_INCLUDED
#define Y_NET_IP_SOCKET_INCLUDED 1

#include "y/net/net.hpp"

namespace upsylon
{
    namespace net
    {
        //! low level socket API
        class ip_socket
        {
        public:
            virtual ~ip_socket() throw();
            explicit ip_socket(const ip_protocol protocol,
                               const ip_version  version);
            
        protected:
            socket_type sock;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(ip_socket);
        };
    }
}



#endif

