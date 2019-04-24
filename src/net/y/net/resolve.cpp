#include "y/net/net.hpp"

#if defined(Y_BSD)
#include <netdb.h>
#endif

#if defined(Y_WIN)

#    if defined(__DMC__)
extern "C" {

    struct addrinfo {
        int ai_flags;
        int ai_family;
        int ai_socktype;
        int ai_protocol;
        size_t ai_addrlen;
        char *ai_canonname;
        struct sockaddr *ai_addr;
        struct addrinfo *ai_next;
    };

    int WSAAPI getaddrinfo(
                           const char *nodename,
                           const char *servname,
                           const struct addrinfo *fmt,
                           struct addrinfo **res
                           );

    void WSAAPI freeaddrinfo( struct addrinfo *ai );


}
#    endif // __DMC__

#if defined(__GNUC__)
extern "C"
{

    int WSAAPI getaddrinfo(const char            *nodename,
                           const char            *servname,
                           const struct addrinfo *fmt,
                           struct addrinfo       **res
                           );

    void WSAAPI freeaddrinfo( struct addrinfo *ai );
}
#endif

#endif // Y_WIN

#include "y/exceptions.hpp"

namespace upsylon
{
    void network:: resolve( net::socket_address &ip, const string &name ) const
    {
        Y_GIANT_LOCK();

        addrinfo fmt;
        memset( &fmt, 0, sizeof(fmt) );
        fmt.ai_family = ip.family();

        addrinfo *ai0 = NULL;
        int       err = ::getaddrinfo(*name, NULL, &fmt, &ai0);
        if( err )
        {
#           if defined(Y_WIN)
            throw win32::exception( err, "::getaddrinfo(%s,%s)" , *name, ip.className() );
#           endif

#           if defined(Y_BSD)
            throw imported::exception( gai_strerror(err), "::getaddrinfo(%s,%s)" , *name, ip.className() );
#           endif
        }

        // TODO: sanity ?
        assert( ai0             != NULL        );
        assert( ai0->ai_addr    != NULL        );
        assert( ai0->ai_addrlen == ip.length() );

        const net16_t p = ip.port; //-- save port
        memcpy( ip.rw(), ai0->ai_addr, ip.length() );
        ip.port = p;               //-- restore port

        ::freeaddrinfo(ai0);
    }

    void network:: resolve( net::socket_address &ip, const char   *s ) const
    {
        const string _(s);
        resolve(ip,_);
    }
}

#include "y/string/convert.hpp"

namespace upsylon
{

    namespace net
    {

        void socket_address:: resolve( const string &xname )
        {
            static const char _sep  = ':';
            const char       *name = *xname;
            char             *psep = (char *) ( strchr(name, _sep ) );
            if(psep)
            {
                port  = bswp( static_cast<uint16_t>( string_convert::to<size_t>(psep+1,"port") ) );
                *psep = 0;
            }
            try
            {
                network::instance().resolve(*this,name);
                if(psep) *psep = _sep;
            }
            catch(...)
            {
                if(psep) *psep = _sep;
                throw;
            }
        }

        void socket_address:: resolve( const char   *xname )
        {
            const string _(xname); resolve(_);
        }
        
    }

}

