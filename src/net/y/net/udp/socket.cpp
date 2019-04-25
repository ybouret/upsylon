
#include "y/net/udp/socket.hpp"


namespace upsylon
{
    namespace net
    {

        udp_socket:: ~udp_socket() throw()
        {
        }

        udp_socket:: udp_socket( const ip_version version ) :
        bsd_socket(udp,version)
        {
        }

        void udp_socket:: sendto( const socket_address &target, const void *data, const size_t size ) const
        {
            Y_GIANT_LOCK();
            assert( !(data==NULL&&size>0) );
            const int msgSize = int(size);
            if(msgSize<0)
            {
                throw upsylon::exception("sendto(%s): message size overflow", target.text() );
            }

            const int msgSent = ::sendto(sock,
                                         (const char *)data,
                                         msgSize,
                                         send_flags,
                                         static_cast<const sockaddr *>(target.ro()),
                                         unsigned( target.length()) );
            if(msgSent!=msgSize)
            {
                throw net::exception( get_last_error_code(), "sendto(%s)", target.text() );
            }
        }

#if defined(Y_BSD)
#define Y_NET_SOCKLEN socklen_t
#endif

#if defined(Y_WIN)
#define Y_NET_SOCKLEN int
#endif

#define Y_NET_UDP_RECVFROM() ::recvfrom(sock, (char *)data,msgSize,recv_flags,  static_cast<sockaddr *>(source.rw()), &addrLen )

        size_t udp_socket:: recvfrom( socket_address &source, void *data, const size_t size ) const
        {
            Y_GIANT_LOCK();
            assert( !(data==NULL&&size>0) );
            const int msgSize = int(size);
            if(msgSize<0)
            {
                throw upsylon::exception("recvfrom: message size overflow" );
            }
            Y_NET_SOCKLEN addrLen = (Y_NET_SOCKLEN)(source.length());

#if defined(Y_WIN)
            const int     msgRecv = Y_NET_UDP_RECVFROM();
            if( SOCKET_ERROR == msgRecv )
            {
                throw net::exception( get_last_error_code(), "::recvfrom" );
            }
            return msgRecv;
#endif

#if defined(Y_BSD)
            int msgRecv = 0;
            while( (msgRecv=Y_NET_UDP_RECVFROM())<0)
            {
                const int err = errno;
                switch( err )
                {
                    case EINTR: continue;
                    default: throw net::exception( err, "::recvfrom" );
                }
            }
            return msgRecv;
#endif
        }

        void udp_socket:: send(const void *data, const size_t size) const { send_(data,size);                }
        void udp_socket:: send(const char *data)                    const { send_(data,length_of(data));     }
        void udp_socket:: send(const memory::ro_buffer &buff)       const { send_(buff.ro(),buff.length() ); }

        size_t udp_socket::recv(void *data, const size_t size) { return recv_(data,size); }
        


    }
}

