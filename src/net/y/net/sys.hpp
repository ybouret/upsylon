//! \file
#ifndef Y_NET_SYS_INCLUDED
#define Y_NET_SYS_INCLUDED 1

#include "y/os/platform.hpp"

#if defined(Y_BSD)
#    if defined(__OpenBSD__)
#        include <sys/types.h>
#        include <sys/socket.h>
#    endif
#    include <netinet/in.h>
#endif

#if defined(Y_WIN)
#    include <winsock2.h>
#    include <ws2tcpip.h>

#        if    defined(__DMC__)
typedef struct in6_addr {
    union {
        UCHAR       Byte[16];
        USHORT      Word[8];
    } u;
} IN6_ADDR;

struct sockaddr_in6 {
    short   sin6_family;
    u_short sin6_port;
    u_long  sin6_flowinfo;
    struct  in6_addr sin6_addr;
    u_long  sin6_scope_id;
};
#        endif // __DMC__

#    if defined(_MSC_VER) || defined(__DMC__)
#        pragma comment(lib,"ws2_32.lib")
#    endif

#endif // defined(Y_WIN)

#endif
