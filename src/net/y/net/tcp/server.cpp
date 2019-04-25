

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

        socket_type tcp_server:: __accept( socket_address &ip ) const
        {
            Y_NET_VERBOSE(std::cerr << "[network.tcp_server(" << (*this)->text() << '@' << bswp((*this)->port) << ").accept]" << std::endl);

            socket_type s      = invalid_socket;
            sa_length_t sa_len = ip.get_sa_length();
            sockaddr   *sa_ptr = ip.get_sa();

#if defined(Y_BSD)
            while( (s = ::accept(sock, sa_ptr, &sa_len)) < 0 )
            {
                const int err = errno;
                switch(err)
                {
                    case EINTR:
                        continue;

                    default:
                        throw  net::exception( err, "::accept()" );
                }
            }
#endif

#if defined(Y_WIN)
            if( INVALID_SOCKET  == (s=::accept(sock, sa_ptr, &sa_len) ) )
            {
                throw net::exception( get_last_error_code(), "::accept()" );
            }
#endif

            assert(s!=invalid_socket);
            return s;
        }

        tcp_client * tcp_server:: accept() const
        {

            socket_addr_ex  xip( ip_addr_none, (**this).version(), 0 );
            socket_address &ip   = *xip;
            tcp_client      *cln = net_object::acquire1<tcp_client>();
            
            try
            {
                socket_type s = __accept(ip);
                return new (cln) tcp_client(s,ip);
            }
            catch(...)
            {
                net_object::release1(cln);
                throw;
            }
        }

    }

}

#include "y/net/tcp/client.hpp"

namespace upsylon
{
    namespace net
    {
        

    }
}

