#include "y/net/socket/bsd.hpp"

#if defined(Y_BSD)
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <errno.h>
#include "y/exceptions.hpp"
#endif

#include "y/codec/base64.hpp"

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
            Y_NET_VERBOSE(std::cerr << "[network.bsd_socket.~<" << name << ">]" << std::endl);
            shutdown(sd_both);
            bsd_close(sock);
            memset( (char *) &name[0], 0, sizeof(name) );
        }


        void bsd_socket:: __fmt_name( char *field, socket_id_t sid) throw()
        {
            assert(field);
            memset(field,0,rnd_bytes);
            size_t pos = 0;
            field[pos++] = '@';
            while(sid)
            {
                assert(pos<rnd_bytes);
                field[pos++] = ios::base64::encode_url[ sid & 0x3f ];
                sid >>= 6;
            }
        }

        void bsd_socket:: on_init()
        {
            __fmt_name((char *)name,uuid);
            Y_NET_VERBOSE(std::cerr << "[network.bsd_socket.name=<" << name << ">]" << std::endl);
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

        static inline
        socket_id_t sock2uuid( const socket_type &s ) throw()
        {
            union {
                socket_id_t id;
                socket_type st;
            } item = { 0 };
            item.st = s;
            return item.id;
        }

#define Y_NET_BSD_CTOR()     \
uuid( sock2uuid(sock)     ),\
hkey( network::hash(sock) ), name()

#define Y_NET_BSD_INI() do { \
Y_NET_VERBOSE(std::cerr << "[network.bsd_socket.init: uuid=" << uuid << ", hkey=" << hkey << "]" << std::endl);\
on_init();        \
} while(false)

        bsd_socket:: bsd_socket( const ip_protocol protocol, const ip_version version) :
        sock( network::instance().open(protocol,version) ),
        Y_NET_BSD_CTOR()
        {
            Y_NET_BSD_INI();
        }

        bsd_socket:: bsd_socket( const socket_type accepted ) :
        sock( accepted ),
        Y_NET_BSD_CTOR()
        {
            Y_NET_BSD_INI();
        }


        void bsd_socket:: blocking(const bool value)
        {
            Y_NET_VERBOSE(std::cerr << "[network.bsd_socket.blocking(<" << name << ">," << (value ? "ON" : "OFF") << ")]" << std::endl);
            Y_GIANT_LOCK();
            assert( invalid_socket != sock );
#if defined(Y_WIN)
            // If iMode == 0, blocking is enabled;
            // If iMode != 0, non-blocking mode is enabled.
            u_long iMode = value ? 0 : 1;
            if( SOCKET_ERROR == ioctlsocket(sock, FIONBIO, &iMode) )
            {
                throw net::exception( Y_NET_LAST_ERROR(), "ioctlsocket(<%s>)",name);
            }
#endif

#if defined(Y_BSD)
            int flags = fcntl(sock, F_GETFL, 0);
            if (flags<0) throw libc::exception( errno, "fcntl(GETFL,<%s>)",name);
            if (value)
                flags &= ~O_NONBLOCK;
            else
                flags |= O_NONBLOCK;
            if( fcntl(sock, F_SETFL, flags) < 0 ) throw libc::exception( errno, "fcntl(SETFL,<%s>)",name);
#endif

        }






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
            Y_NET_VERBOSE(std::cerr << "[network.bsd_socket.shutdown(" << sd_text(how) << ")]" << std::endl);

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
            Y_NET_VERBOSE(std::cerr << "[network.bsd_socket.setopt(<" << name << ">," << sockopt_level(level) << "," << sockopt_name(optname) << ")]" << std::endl);
            Y_GIANT_LOCK();
            assert(invalid_socket!=sock);
            if(optval==0||optlen<=0)
            {
                throw upsylon::exception("bsd_socket::setopt(invalid optval/optlen for socket=<%s>)",name);
            }

#if defined(Y_BSD)
            if( ::setsockopt(sock, level, optname, optval, static_cast<socklen_t>(optlen) ) < 0 )
            {
                throw net::exception( Y_NET_LAST_ERROR(), "setsockopt(<%s>)",name);
            }
#endif

#if defined(Y_WIN)
            if( SOCKET_ERROR == ::setsockopt(sock,level,optname, (const char *)optval, static_cast<int>(optlen) ) )
            {
                throw net::exception( Y_NET_LAST_ERROR(), "setsockopt(<%s>)",name);
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
            Y_NET_VERBOSE(std::cerr << "[network.bsd_socket.getopt(<" << name << ">," << sockopt_level(level) << "," << sockopt_name(optname) << ")]" << std::endl);
            Y_GIANT_LOCK();
            assert(invalid_socket!=sock);
            if(optval==0||optlen<=0)
            {
                throw upsylon::exception("ip_socket::getopt(invalid optval/optlen for <%s>)",name);
            }
            
            sa_length_t optLen = static_cast<sa_length_t>(optlen);

#if defined(Y_BSD)
            if( ::getsockopt(sock, level, optname, optval, &optLen ) < 0 )
            {
                throw net::exception(Y_NET_LAST_ERROR(), "::getsockopt(<%s>)",name);
            }
#endif

#if defined(Y_WIN)
            if( SOCKET_ERROR == ::getsockopt(sock,level,optname, (char *)optval,&optLen) )
            {
                throw net::exception( Y_NET_LAST_ERROR(), "::getsockopt(<%s>)",name);
            }
#endif
        }

       

        bool bsd_socket:: test(const int level, const int optname) const
        {
            return getopt<socket_boolean>(level,optname) != socket_false;
        }

        int bsd_socket:: sndbuf() const { return getopt<int>(SOL_SOCKET,SO_SNDBUF); }

        int bsd_socket:: rcvbuf() const { return getopt<int>(SOL_SOCKET,SO_RCVBUF); }

        
        const size_t & bsd_socket:: key() const throw()
        {
            return hkey;
        }

#define Y_NET_SOCKOPT(ID) case ID: return #ID
        
        const char *bsd_socket:: sockopt_level(const int level)  throw()
        {
            switch(level)
            {
                    Y_NET_SOCKOPT(SOL_SOCKET);
                default:
                    break;
            }
            return "UNKNOWN_LEVEL";
        }
        
        const char *bsd_socket:: sockopt_name(const int optname)  throw()
        {
            switch(optname)
            {
                    Y_NET_SOCKOPT(SO_REUSEADDR);
                    Y_NET_SOCKOPT(SO_SNDBUF);
                    Y_NET_SOCKOPT(SO_RCVBUF);
                default:
                    break;
            }
            return "UNKNOWN_NAME";
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


