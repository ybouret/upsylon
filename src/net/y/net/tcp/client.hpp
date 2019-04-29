//! \file
#ifndef Y_NET_TCP_CLIENT_INCLUDED
#define Y_NET_TCP_CLIENT_INCLUDED 1

#include "y/net/tcp/socket.hpp"
#include "y/net/socket/io-params.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon
{
    namespace net
    {
        
        //! TCP client
        class tcp_client : public socket_addr_ex, public tcp_socket, public socket_io_params
        {
        public:
            virtual ~tcp_client() throw();                 //!< destructor
            explicit tcp_client(const socket_address &ip); //!< connect to a precomputed address
            explicit tcp_client(const string &xname, const ip_version version = v4); //!< connect by name resolution
            explicit tcp_client(const char   *xname, const ip_version version = v4); //!< connect by name resolution

            size_t send( const void *data, const size_t size ) const; //!< try to read at most size bytes (0:disconnect), truncated to positive int
            size_t recv( void       *data, const size_t size ) const; //!< try to read at most size bytes (0:disconnect), truncated to positive int

            size_t send_block(const void *data,const size_t size) const; //!< best effort sending
            size_t send_block(const char *data) const;                   //!< best effort sending
            size_t send_block(const memory::ro_buffer &buff) const;      //!< best effort sending

            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(tcp_client);
            void start();
            explicit tcp_client(const socket_type    &s,
                                const socket_address &ip);
            friend class tcp_server;
        };

        typedef arc_ptr<tcp_client> tcp_link; //!< alias

    }
}

#endif
