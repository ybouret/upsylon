

#include "y/net/tcp/server.hpp"

namespace upsylon
{
    namespace net
    {
        tcp_server:: ~tcp_server() throw()
        {
            Y_NET_VERBOSE(std::cerr << "[network.tcp_server.quit]" << std::endl);
        }

        tcp_server:: tcp_server( const socket_address &ip, const unsigned pending) :
        socket_addr_ex(ip), tcp_socket( (**this).version() )
        {
            start( pending );
        }

        tcp_server:: tcp_server( const uint16_t user_port, const unsigned pending, const ip_version version) :
        socket_addr_ex(ip_addr_any,version,user_port),tcp_socket( (**this).version() )
        {
            start(pending);
        }

        void tcp_server:: start(const unsigned pending)
        {
            Y_NET_VERBOSE(std::cerr << "[network.tcp_server.init]" << std::endl);

            // TODO: check pending overflow ?
            const socket_address &self = **this;

            self.bind(sock);

            Y_NET_VERBOSE(std::cerr << "[network.listen(" << self.text() << '@' << bswp(self.port) <<  ")]" << std::endl);
            Y_GIANT_LOCK();

#if defined(Y_WIN)
            if( SOCKET_ERROR == ::listen(sock,pending) )
            {
                throw net::exception( get_last_error_code(), "::listen(%s)", self.text() );
            }
#endif

#if defined(Y_BSD)
            while( ::listen(sock,pending) < 0 )
            {
                const int err = errno;
                switch(err)
                {
                    case EINTR:
                        continue;

                    default:
                        throw  net::exception( err, "::listen(%s)", self.text() );
                }
            }
#endif
        }

    }

}

