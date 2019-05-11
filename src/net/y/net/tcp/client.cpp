
#include "y/net/tcp/client.hpp"
#include "y/type/ints-chkbnd.hpp"

#if defined(Y_BSD)
#include <cerrno>
#endif

namespace upsylon
{
    namespace net
    {

        tcp_client:: ~tcp_client() throw()
        {
            Y_NET_VERBOSE(std::cerr << "[network.tcp_client.quit]" << std::endl);
        }

#define Y_NET_TCP_CLIENT( INI ) socket_addr_ex INI,  tcp_socket( (**this).version() )

        tcp_client:: tcp_client(const socket_address &ip ) :                     Y_NET_TCP_CLIENT( (ip) )            { start(); }
        tcp_client:: tcp_client(const string &xname, const ip_version version) : Y_NET_TCP_CLIENT( (xname,version) ) { start(); }
        tcp_client:: tcp_client(const char   *xname, const ip_version version) : Y_NET_TCP_CLIENT( (xname,version) ) { start(); }

        void tcp_client:: start()
        {
            Y_NET_VERBOSE(std::cerr << "[network.tcp_client.init]" << std::endl);
            (*this)->connect(sock);
        }

        tcp_client:: tcp_client(const socket_type    &s,
                                const socket_address &ip) :
        socket_addr_ex(ip),
        tcp_socket(s)
        {
            
        }

        size_t tcp_client:: send( const void *data, const size_t size ) const
        {
            Y_GIANT_LOCK();
            assert( !(data==NULL&&size>0) );

            const int sz = check_bound<int,size_t>(size);
            Y_NET_VERBOSE(const socket_address &ip = **this; std::cerr << "[net.tcp.client.send #" << sz << " ->" << ip << "]" << std::endl);

#if defined(Y_BSD)
            int            ns = 0;
            while( (ns= ::send(sock,data,sz,send_flags)) < 0 )
            {
                const int err = errno;
                switch (err) {
                    case EINTR:continue;
                    default: throw net::exception(err,"::send(%s)", (**this).text() );
                }
            }
            return ns;
#endif

#if defined(Y_WIN)
            const int ns = ::send(sock,(const char*)data,sz,send_flags);
            if( ns == SOCKET_ERROR )
            {
                throw net::exception( Y_NET_LAST_ERROR(), "::send(%s)", (**this).text() );
            }
            return ns;
#endif
        }

        size_t tcp_client:: recv( void *data, const size_t size ) const
        {
            Y_GIANT_LOCK();
            const int sz = check_bound<int,size_t>(size);
            Y_NET_VERBOSE(std::cerr << "[net.tcp.client.recv(up to " << sz << ")]" << std::endl);

#if defined(Y_BSD)
            int            nr = 0;
            while( (nr= ::recv(sock,data,sz,send_flags)) < 0 )
            {
                const int err = errno;
                switch (err) {
                    case EINTR:continue;
                    default: throw net::exception(err,"::recv(%s)", (**this).text() );
                }
            }
            return nr;
#endif

#if defined(Y_WIN)
            const int nr = ::recv(sock,(char*)data,sz,send_flags);
            if( nr == SOCKET_ERROR )
            {
                throw net::exception( Y_NET_LAST_ERROR(), "::recv(%s)", (**this).text() );
            }
            return nr;
#endif
        }


        size_t tcp_client:: send_block(const void *data,const size_t size) const
        {
            const uint8_t *addr = (const uint8_t *)data;
            size_t         todo = size;
            while(todo>0)
            {
                const size_t sent = send(addr,todo);
                if(sent<=0) break;
                todo -= sent;
                addr += sent;
            }
            return size-todo;
        }

        size_t tcp_client:: send_block(const char *data) const
        {
            return send_block( data, length_of(data) );
        }

        size_t tcp_client:: send_block(const memory::ro_buffer &buff) const
        {
            return send_block( buff.ro(), buff.length() );
        }


    }

}

