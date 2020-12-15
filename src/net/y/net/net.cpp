#include "y/net/net.hpp"
#include "y/exceptions.hpp"

#if defined(Y_BSD)
#include <csignal>
#include <unistd.h>
#include <cerrno>
#endif

#include "y/type/block/zset.hpp"
#include <iostream>

namespace upsylon
{

    Y_SINGLETON_IMPL(network);
    
    namespace
    {

#if defined(Y_WIN)
        static WSADATA wsa;
#endif


    }


    bool network::verbose = false;


    network:: ~network() throw()
    {
        Y_NET_VERBOSE("[network.cleanup]");

        //----------------------------------------------------------------------
        //
        // cleanup local stuff...
        //
        //----------------------------------------------------------------------



        //----------------------------------------------------------------------
        //
        // cleanup system stuff
        //
        //----------------------------------------------------------------------
#if defined(Y_WIN)
        ::WSACleanup();
#endif
    }


    network:: network()
    {
        Y_NET_VERBOSE("[network.startup]");
        Y_GIANT_LOCK();

        //----------------------------------------------------------------------
        //
        // system dependent initializations
        //
        //----------------------------------------------------------------------
#if defined(Y_WIN)
        memset(&wsa, 0, sizeof(WSADATA) );
        if( :: WSAStartup( MAKEWORD(2,2), &wsa ) !=  0 )
        {
            throw win32::exception( ::WSAGetLastError(), "::WSAStartup" );
        }
#endif

#if defined(Y_BSD)
        signal( SIGPIPE, SIG_IGN );
#endif

        //----------------------------------------------------------------------
        //
        // local stuff...
        //
        //----------------------------------------------------------------------
        
    }


    const uint16_t network:: reserved_port   = IPPORT_RESERVED;
#       if defined(Y_WIN) || defined(__FreeBSD__)
    const uint16_t network:: first_user_port = IPPORT_RESERVED;
#        else
    const uint16_t network:: first_user_port = IPPORT_USERRESERVED;
#        endif

    const uint16_t network:: user_port_width = (network::final_user_port-network::first_user_port)+1;


    net::ip_version network:: ip_version(const string &ipv)
    {
        if(ipv=="v4") return net::v4;
        if(ipv=="v6") return net::v6;
        throw exception("invalid ip version '%s'", *ipv);
    }

    net::ip_version network:: ip_version(const char *ipv)
    {
        const string _(ipv); return ip_version(_);
    }



}


#include "y/memory/zblock.hpp"
#include "y/memory/allocator/pooled.hpp"

namespace upsylon
{

    string network:: get_host_name() const
    {
        Y_GIANT_LOCK();
        size_t len = 256;

#if defined(Y_BSD)
        for(;;)
        {
            zblock<char,memory::pooled> blk( len );
            if( ::gethostname( *blk, blk.length() ) != 0)
            {
                len *= 2;
                continue;
            }
            return string( *blk );
        }
#endif

#if defined(Y_WIN)
        for(;;) {
            zblock<char,memory::pooled> blk( len );
            if( ::gethostname( *blk, int(blk.length()) ) == SOCKET_ERROR ) {
                const DWORD err = ::WSAGetLastError();
                if( err != WSAEFAULT )
                    throw   win32::exception( err, "::gethostname" );
                len *= 2;
                continue;
            }
            return string( *blk );
        }
#endif

    }

}


namespace upsylon
{

    namespace net
    {

#if defined(Y_WIN)
        const  socket_type  invalid_socket = INVALID_SOCKET;
#endif

#if defined(Y_BSD)
        const  socket_type invalid_socket = -1;
#endif

        static
        socket_type bsd_acquire_socket(const int proto_family,
                                       const int type,
                                       const int protocol)
        {
            Y_NET_VERBOSE("[network.bsd_socket.open]");


            Y_GIANT_LOCK();

            while( true )
            {
                socket_type s = ::socket(proto_family,type,protocol);
                if( s == invalid_socket )
                {
                    const int err = Y_NET_LAST_ERROR();

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


    }


    net::socket_type network:: open(const net::ip_protocol proto, const net::ip_version version)
    {
        static const char unk[] = "unknown";

        int         pf  = -1;
        const char *ipv = unk;
        switch( version )
        {
            case net::v4: pf = PF_INET;  ipv= net::ipv4::class_name; break;
            case net::v6: pf = PF_INET6; ipv= net::ipv4::class_name; break;
        }

        int st = -1;
        int pr = -1;
        const char *ppv = unk;
        switch( proto )
        {
            case net::tcp: st = SOCK_STREAM; pr=IPPROTO_TCP; ppv = "tcp"; break;
            case net::udp: st = SOCK_DGRAM;  pr=IPPROTO_UDP; ppv = "udp"; break;
        }
        Y_NET_VERBOSE("[network.open(" << ipv << "," << ppv << ")]"<<std::endl);
        return net::bsd_acquire_socket(pf,st,pr);
    }

    
}

#include "y/net/socket/bsd.hpp"

namespace upsylon
{

    namespace net
    {

        void *bsd_socket:: addr2addr(void *ptr) throw()
        {
            assert(ptr);
            return ptr;
        }


    }
}
