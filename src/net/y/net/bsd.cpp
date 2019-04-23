#include "y/net/bsd.hpp"
#include "y/net/sys.hpp"
#include "y/concurrent/mutex.hpp"

#if defined(Y_BSD)
#include <cerrno>
#include <unistd.h>
#endif

namespace upsylon
{
    namespace net
    {
#if defined(Y_WIN)
        const bsd::socket_type bsd::invalid_socket = INVALID_SOCKET;
#endif

#if defined(Y_BSD)
        const bsd::socket_type bsd::invalid_socket = -1;
#endif

        bsd::socket_type bsd::acquire_socket(const int proto_family,
                                             const int type,
                                             const int protocol)
        {
            Y_GIANT_LOCK();

            while( true )
            {
                socket_type s = ::socket(proto_family,type,protocol);
                if( s == invalid_socket )
                {
                    const int err = net::get_last_error_code();

#if defined(Y_BSD)
                    switch( err )
                    {
                        case EINTR: continue;
                        default:    break;
                    }
#endif

                    throw net::exception( err, "socket" );
                }

                return s;
            }
        }

        void bsd::release_socket(socket_type &s) throw()
        {
            assert( invalid_socket != s );
#if defined(Y_BSD)
            (void) ::close(s);
#endif

#if defined(Y_WIN)
            (void)::closesocket(s);
#endif
            s = invalid_socket;
        }

    }

}
