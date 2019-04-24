//! \file
#ifndef Y_NET_UDP_SOCKET_INCLUDED
#define Y_NET_UDP_SOCKET_INCLUDED 1

#include "y/net/socket/bsd.hpp"
#include "y/net/socket/io-params.hpp"

namespace upsylon
{
    namespace net
    {

        //! base class for UDP socket
        class udp_socket : public bsd_socket, public socket_io_params
        {
        public:
            virtual ~udp_socket() throw();

            virtual void   send_block(const void *data, const size_t size ) const = 0;
            virtual size_t recv(void *data, const size_t size) = 0;
            
            void           send(const char *data) const;
            void           send(const memory::ro_buffer &buff) const;
            
        protected:
            explicit udp_socket(const ip_version version); //!< setup+version

            void   sendto(const socket_address &target, const void *data, const size_t size ) const;
            size_t recvfrom(socket_address     &source, void       *data, const size_t size ) const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(udp_socket);
        };

    }
}

#endif

