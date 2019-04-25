
//! \file
#ifndef Y_NET_UDP_SERVER_INCLUDED
#define Y_NET_UDP_SERVER_INCLUDED 1

#include "y/net/udp/socket.hpp"

namespace upsylon
{
    namespace net
    {

        //! UDP server
        class udp_server : public socket_addr_ex, public udp_socket
        {
        public:
            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual ~udp_server() throw();                                     //!< cleanup
            virtual size_t recv_( void *data,       const size_t size);        //!< recv and set peer to source
            virtual void   send_( const void *data, const size_t size) const;  //!< send to peer

            //__________________________________________________________________
            //
            // non virtual interface
            //__________________________________________________________________
            explicit udp_server( const socket_address &ip );                  //!< bind to specific address
            explicit udp_server( const uint16_t user_port, const ip_version); //!< bind to ANY and port
            
            socket_addr_ex peer; //!< peer address, use as target or incoming source

            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(udp_server);
        };

    }

}

#endif


