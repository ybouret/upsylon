#include "y/net/ip-socket.hpp"

#if defined(Y_BSD)
#include <unistd.h>
#endif

namespace upsylon
{
    namespace net
    {
        ip_socket:: ~ip_socket() throw()
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


        ip_socket:: ip_socket( const ip_protocol protocol, const ip_version version) :
        sock( network::instance().open(protocol,version) )
        {
        }
        
    }
}


