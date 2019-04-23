#include "y/net/ip-socket.hpp"

#if defined(Y_BSD)
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <errno.h>
#endif

namespace upsylon
{
    namespace net
    {
        static inline
        void bsd_close( socket_type &sock ) throw()
        {
            assert( invalid_socket != sock );
#if defined(Y_BSD)
            (void) ::close(sock);
#endif

#if defined(Y_WIN)
            (void)::closesocket(sock);
#endif
            sock = invalid_socket;
        }

        ip_socket:: ~ip_socket() throw()
        {
            shutdown(sd_both);
            bsd_close(sock);
        }


        ip_socket:: ip_socket( const ip_protocol protocol, const ip_version version) :
        sock( network::instance().open(protocol,version) )
        {
            try
            {
                on(SOL_SOCKET,SO_REUSEADDR);
            }
            catch(...)
            {
                bsd_close(sock);
                throw;
            }
        }


        void ip_socket:: async()
        {
            Y_GIANT_LOCK();

            assert( invalid_socket != sock );
#if (Y_WIN)
            // If iMode == 0, blocking is enabled;
            // If iMode != 0, non-blocking mode is enabled.
            u_long iMode = 1;
            if( SOCKET_ERROR == ioctlsocket(sock, FIONBIO, &iMode) )
            {
                throw net::exception( net::get_last_error_code(), "async/ioctlsocket" );
            }
#endif

#if (Y_BSD)
            if( fcntl( sock, F_SETFL, O_NONBLOCK) < 0 )
            {
                throw net::exception( errno, "async/fcntl");
            }
#endif
        }

        void ip_socket:: shutdown(const shutdown_type how) throw()
        {
            assert( sock != invalid_socket );

#if (Y_WIN)
            switch (how) {
                case sd_send:
                    (void) :: shutdown(sock,SD_SEND);
                    break;

                case sd_recv:
                    (void) :: shutdown(sock,SD_RECEIVE);
                    break;

                case sd_both:
                    (void) :: shutdown(sock,SD_BOTH);
                    break;
            }
#endif

#if (Y_BSD)
            switch (how) {
                case sd_recv:
                    (void) :: shutdown(sock,0);
                    break;

                case sd_send:
                    (void) :: shutdown(sock,1);
                    break;

                case sd_both:
                    (void) :: shutdown(sock,2);
                    break;
            }
#endif

        }

#if defined(Y_BSD)
        const socket_boolean socket_true  = 1;
        const socket_boolean socket_false = 0;
#endif

#if defined(Y_WIN)
        const socket_boolean socket_true  = TRUE;
        const socket_boolean socket_false = FALSE;
#endif

        void ip_socket:: setopt(const int level, const int optname, const void *optval, const unsigned optlen)
        {
            Y_GIANT_LOCK();
            assert(invalid_socket!=sock);
            if(optval==0||optlen<=0)
            {
                throw upsylon::exception("ip_socket::setopt(invalid optval/optlen");
            }
#if defined(Y_BSD)
            if( ::setsockopt(sock, level, optname, optval, static_cast<socklen_t>(optlen) ) < 0 )
            {
                throw net::exception( net::get_last_error_code(), "setsockopt");
            }
#endif

#if defined(Y_WIN)
            if( SOCKET_ERROR == ::setsockopt(sock,level,optname, (const char *)optval, static_cast<int>(optlen) ) )
            {
                throw net::exception( net::get_last_error_code(), "setsockopt");
            }
#endif

        }

        void ip_socket:: on(  const int level, const int optname )  { setopt<socket_boolean>(level,optname,socket_true);  }
        void ip_socket:: off( const int level, const int optname )  { setopt<socket_boolean>(level,optname,socket_false); }


    }
}


