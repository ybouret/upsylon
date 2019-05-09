#include "y/net/net.hpp"
#include "y/exceptions.hpp"

#if defined(Y_BSD)
#include <csignal>
#include <unistd.h>
#include <cerrno>
#endif

#include "y/memory/arena-of.hpp"
#include "y/memory/io.hpp"

namespace upsylon
{

    namespace
    {

#if defined(Y_WIN)
        static WSADATA wsa;
#endif

        typedef memory::arena_of<net::byte_node> byte_node_arena;
        uint64_t __byte_node_arena[Y_U64_FOR_ITEM(byte_node_arena)];
    }

    net::byte_node * network:: acquire_byte_node() const
    {
        Y_LOCK(access);
        static byte_node_arena &a = *memory::io::__force<byte_node_arena>(__byte_node_arena);
        return a.acquire();
    }

    void network:: release_byte_node( net::byte_node *p) const throw()
    {
        Y_LOCK(access);
        static byte_node_arena &a = *memory::io::__force<byte_node_arena>(__byte_node_arena);
        a.release(p);
    }

    bool network::verbose = false;


    network:: ~network() throw()
    {
        Y_NET_VERBOSE(std::cerr<< "[network.cleanup]" << std::endl);

        //----------------------------------------------------------------------
        //
        // cleanup local stuff...
        //
        //----------------------------------------------------------------------
        destruct( memory::io::__force<byte_node_arena>(__byte_node_arena) );
        memset( __byte_node_arena, 0, sizeof(__byte_node_arena) );


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
        Y_NET_VERBOSE(std::cerr << "[network.startup]" << std::endl);
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
        memset( __byte_node_arena, 0, sizeof(__byte_node_arena) );
        new ( memory::io::__force<byte_node_arena>(__byte_node_arena)) byte_node_arena(Y_CHUNK_SIZE);
    }


    const uint16_t network:: reserved_port   = IPPORT_RESERVED;
#       if defined(Y_WIN) || defined(__FreeBSD__)
    const uint16_t network:: first_user_port = IPPORT_RESERVED;
#        else
    const uint16_t network:: first_user_port = IPPORT_USERRESERVED;
#        endif

    const uint16_t network:: user_port_width = (network::final_user_port-network::first_user_port)+1;

}


#include "y/memory/buffers.hpp"

namespace upsylon
{

    string network:: get_host_name() const
    {
        Y_GIANT_LOCK();
        size_t len = 256;

#if defined(Y_BSD)
        for(;;)
        {
            memory::pooled_buffer_of<char> blk( len );
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
            memory::pooled_buffer_of<char> blk( len );
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
            Y_NET_VERBOSE(std::cerr << "[network.bsd_socket.open]" << std::endl);


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
        Y_NET_VERBOSE(std::cerr << "[network.open(");
        int pf  = -1;
        switch( version )
        {
            case net::v4: pf = PF_INET;  Y_NET_VERBOSE(std::cerr << net::ipv4::class_name); break;
            case net::v6: pf = PF_INET6; Y_NET_VERBOSE(std::cerr << net::ipv6::class_name); break;
        }

        int st = -1;
        int pr = -1;
        switch( proto )
        {
            case net::tcp: st = SOCK_STREAM; pr=IPPROTO_TCP; Y_NET_VERBOSE(std::cerr << ",tcp"); break;
            case net::udp: st = SOCK_DGRAM;  pr=IPPROTO_UDP; Y_NET_VERBOSE(std::cerr << ",udp"); break;
        }
        Y_NET_VERBOSE(std::cerr << ")]"<<std::endl);
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
