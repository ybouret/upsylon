#include "y/net/socket/bsd.hpp"

#if defined(Y_BSD)
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <errno.h>
#include "y/exceptions.hpp"
#endif

namespace upsylon
{
    namespace net
    {
        static inline
        void bsd_close( socket_type &sock ) throw()
        {
            Y_NET_VERBOSE(std::cerr << "[network.bsd_socket.quit]" << std::endl);

            assert( invalid_socket != sock );
#if defined(Y_BSD)
            (void) ::close(sock);
#endif

#if defined(Y_WIN)
            (void)::closesocket(sock);
#endif
            sock = invalid_socket;
        }

        bsd_socket:: ~bsd_socket() throw()
        {
            shutdown(sd_both);
            bsd_close(sock);
        }

        void bsd_socket:: on_init()
        {
            try
            {
                on(SOL_SOCKET,SO_REUSEADDR);
                assert(test(SOL_SOCKET,SO_REUSEADDR));
            }
            catch(...)
            {
                bsd_close(sock);
                throw;
            }
        }

        bsd_socket:: bsd_socket( const ip_protocol protocol, const ip_version version) :
        sock( network::instance().open(protocol,version) )
        {
            Y_NET_VERBOSE(std::cerr << "[network.bsd_socket.init]" << std::endl);
            on_init();
        }

        bsd_socket:: bsd_socket( const socket_type accepted ) :
        sock( accepted )
        {
            Y_NET_VERBOSE(std::cerr << "[network.bsd_socket.init/accepted]" << std::endl);
            on_init();
        }


        void bsd_socket:: blocking(const bool value)
        {
            Y_NET_VERBOSE(std::cerr << "[network.bsd.socket.blocking(" << value << ")]" << std::endl);
            Y_GIANT_LOCK();
            assert( invalid_socket != sock );
#if defined(Y_WIN)
            // If iMode == 0, blocking is enabled;
            // If iMode != 0, non-blocking mode is enabled.
            u_long iMode = value ? 0 : 1;
            if( SOCKET_ERROR == ioctlsocket(sock, FIONBIO, &iMode) )
            {
                throw net::exception( Y_NET_LAST_ERROR(), "ioctlsocket" );
            }
#endif

#if defined(Y_BSD)
            int flags = fcntl(sock, F_GETFL, 0);
            if (flags<0) throw libc::exception( errno, "fcntl(GETFL)");
            if (value)
                flags &= ~O_NONBLOCK;
            else
                flags |= O_NONBLOCK;
            if( fcntl(sock, F_SETFL, flags) < 0 ) throw libc::exception( errno, "fcntl(SETFL)");
#endif

        }

#if 0
        void bsd_socket:: async()
        {
            Y_NET_VERBOSE(std::cerr << "[network.bsd.socket.ASYNC]" << std::endl);
            Y_GIANT_LOCK();

            assert( invalid_socket != sock );
#if defined(Y_WIN)
            // If iMode == 0, blocking is enabled;
            // If iMode != 0, non-blocking mode is enabled.
            u_long iMode = 1;
            if( SOCKET_ERROR == ioctlsocket(sock, FIONBIO, &iMode) )
            {
                throw net::exception( Y_NET_LAST_ERROR(), "async/ioctlsocket" );
            }
#endif

#if defined(Y_BSD)
            if( fcntl( sock, F_SETFL, O_NONBLOCK) < 0 )
            {
                throw net::exception( errno, "async/fcntl");
            }
#endif
        }
#endif




        const char * bsd_socket:: sd_text(const shutdown_type how) throw()
        {
            switch (how) {
                case sd_send:
                    return "SEND";

                case sd_recv:
                    return "RECV";

                case sd_both:
                    return "BOTH";
            }
            return "????";
        }

        void bsd_socket:: shutdown(const shutdown_type how) throw()
        {
            assert( sock != invalid_socket );
            Y_NET_VERBOSE(std::cerr << "[network.bsd.shutdown(" << sd_text(how) << ")]" << std::endl);

#if defined(Y_WIN)
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

#if defined(Y_BSD)
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

        void bsd_socket:: setopt(const int level, const int optname, const void *optval, const unsigned optlen)
        {
            Y_NET_VERBOSE(std::cerr << "[network.setopt]" << std::endl);
            Y_GIANT_LOCK();
            assert(invalid_socket!=sock);
            if(optval==0||optlen<=0)
            {
                throw upsylon::exception("bsd_socket::setopt(invalid optval/optlen");
            }

            //__show_opt(std::cerr<<"setopt:[",optval,optlen) << "]" << std::endl;

#if defined(Y_BSD)
            if( ::setsockopt(sock, level, optname, optval, static_cast<socklen_t>(optlen) ) < 0 )
            {
                throw net::exception( Y_NET_LAST_ERROR(), "setsockopt");
            }
#endif

#if defined(Y_WIN)
            if( SOCKET_ERROR == ::setsockopt(sock,level,optname, (const char *)optval, static_cast<int>(optlen) ) )
            {
                throw net::exception( Y_NET_LAST_ERROR(), "setsockopt");
            }
#endif

        }

        void bsd_socket:: on(  const int level, const int optname )  { setopt<socket_boolean>(level,optname,socket_true);  }
        void bsd_socket:: off( const int level, const int optname )  { setopt<socket_boolean>(level,optname,socket_false); }


        void bsd_socket:: getopt(const int      level,
                                 const int      optname,
                                 void          *optval,
                                 const unsigned optlen) const
        {
            Y_NET_VERBOSE(std::cerr << "[network.getopt]" << std::endl);
            Y_GIANT_LOCK();
            assert(invalid_socket!=sock);
            if(optval==0||optlen<=0)
            {
                throw upsylon::exception("ip_socket::getopt(invalid optval/optlen");
            }
            
            sa_length_t optLen = static_cast<sa_length_t>(optlen);

#if defined(Y_BSD)
            if( ::getsockopt(sock, level, optname, optval, &optLen ) < 0 )
            {
                throw net::exception(Y_NET_LAST_ERROR(), "::getsockopt");
            }
#endif

#if defined(Y_WIN)
            if( SOCKET_ERROR == ::getsockopt(sock,level,optname, (char *)optval,&optLen) )
            {
                throw net::exception( Y_NET_LAST_ERROR(), "::getsockopt");
            }
#endif
        }

       

        bool bsd_socket:: test(const int level, const int optname) const
        {
            return getopt<socket_boolean>(level,optname) != socket_false;
        }

        int bsd_socket:: sndbuf() const { return getopt<int>(SOL_SOCKET,SO_SNDBUF); }

        int bsd_socket:: rcvbuf() const { return getopt<int>(SOL_SOCKET,SO_RCVBUF); }


        const socket_type & bsd_socket:: key() const throw()
        {
            return sock;
        }

    }

}

#include "y/code/utils.hpp"
namespace upsylon
{
    namespace net
    {
        std::ostream & bsd_socket:: __show_opt(std::ostream  &os,
                                               const void    *optval,
                                               const unsigned optlen)
        {
            assert(optval);
            assert(optlen);
            const uint8_t *p = static_cast<const uint8_t *>(optval);
            for(size_t i=0;i<optlen;++i)
            {
                os << hexadecimal::lowercase[p[i]];
            }
            return os;
        }



    }
}


