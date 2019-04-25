//! \file
#ifndef Y_NET_UDP_CLIENT_INCLUDED
#define Y_NET_UDP_CLIENT_INCLUDED 1

#include "y/net/udp/socket.hpp"

namespace upsylon
{
    namespace net
    {
        //! a client is a socket with a target address, and a last recv IP
        class udp_client :  public socket_addr_ex, public udp_socket
        {
        public:
            virtual ~udp_client() throw(); //!< destructor
            explicit udp_client(const socket_address &ip); //!< initialize to target
            explicit udp_client(const string &xname, const ip_version version = v4); //!< initialize to name
            explicit udp_client(const char   *xname, const ip_version version = v4); //!< initialize to name

            virtual void   send_(const void *data, const size_t size) const; //!< sendto target
            virtual size_t recv_(void *data, const size_t size);             //!< recfrom last_recv_ip

            socket_addr_ex last_recv_ip; //!< store origin of last recv

        private:
            Y_DISABLE_COPY_AND_ASSIGN(udp_client);
        };
    }
}

#endif

