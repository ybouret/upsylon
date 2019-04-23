//! \file
#ifndef Y_NET_BSD_INCLUDED
#define Y_NET_BSD_INCLUDED 1

#include "y/net/types.hpp"

namespace upsylon
{
    namespace net
    {

        struct bsd
        {
#if defined(Y_WIN)
            typedef SOCKET socket_type;
#endif

#if defined(Y_BSD)
            typedef int    socket_type;
#endif
            static const socket_type invalid_socket;

            static socket_type acquire_socket(const int proto_family, const int type, const int protocol );
            static void        release_socket( socket_type &s ) throw();

        };

    }
}

#endif

