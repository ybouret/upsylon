//! \file
#ifndef Y_NET_TCP_SOCKET_INCLUDED
#define Y_NET_TCP_SOCKET_INCLUDED 1

#include "y/net/socket/bsd.hpp"

namespace upsylon
{
    namespace net
    {

        //! base class for TCP socket
        class tcp_socket : public bsd_socket
        {
        public:
            virtual ~tcp_socket() throw(); //! destructor

        protected:
            explicit tcp_socket( const ip_version   ); //!< setup+version
            explicit tcp_socket( const socket_type &); //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(tcp_socket);
        };

    }
}

#endif

