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
            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual ~udp_socket() throw();                                        //!< destructor
            virtual void   send_(const void *data, const size_t size ) const = 0; //!< send a block of data
            virtual size_t recv_(void       *data, const size_t size)        = 0; //!< recv a block of data

            //__________________________________________________________________
            //
            // non-virtual interface
            //__________________________________________________________________
            void           send(const void *data, const size_t size) const; //!< wrapper to send_
            void           send(const char *data)                    const; //!< send text
            void           send(const memory::ro_buffer &buff)       const; //!< send buffer

            size_t         recv(void *data, const size_t size);             //!< wrapper to recv_
            

        protected:
            explicit udp_socket(const ip_version version); //!< setup+version

            void   sendto(const socket_address &target, const void *data, const size_t size ) const; //!< sending to target
            size_t recvfrom(socket_address     &source, void       *data, const size_t size ) const; //!< receiving from source. 0 bytes mean terminated...

        private:
            Y_DISABLE_COPY_AND_ASSIGN(udp_socket);
        };

    }
}

#endif

