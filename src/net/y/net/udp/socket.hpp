//! \file
#ifndef Y_NET_UDP_SOCKET_INCLUDED
#define Y_NET_UDP_SOCKET_INCLUDED 1

#include "y/net/socket/bsd.hpp"

namespace upsylon
{
    namespace net
    {

        //! base class for UDP socket
        class udp_socket : public bsd_socket
        {
        public:
            virtual ~udp_socket() throw();

        protected:
            explicit udp_socket(const ip_version version); //!< setup+version


        private:
            Y_DISABLE_COPY_AND_ASSIGN(udp_socket);
        };

    }
}

#endif

