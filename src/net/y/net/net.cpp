#include "y/net/net.hpp"
#include "y/exceptions.hpp"

#if defined(Y_BSD)
#include <csignal>
#include <unistd.h>
#endif


namespace upsylon
{

    bool network::verbose = false;

#if defined(Y_WIN)
    static WSADATA wsa;
#endif
    network:: ~network() throw()
    {
#if defined(Y_WIN)
        ::WSACleanup();
#endif
        if(verbose) { std::cerr << "[network.cleanup]" << std::endl; }
    }


    network:: network()
    {
        Y_GIANT_LOCK();
        if(verbose) { std::cerr << "[network.startup"; }

#if defined(Y_WIN)
        memset(&wsa, 0, sizeof(WSADATA) );
        if( :: WSAStartup( MAKEWORD(2,2), &wsa ) !=  0 )
        {
            throw win32::exception( ::WSAGetLastError(), "WSAStartup" );
        }
#endif

#if defined(Y_BSD)
        signal( SIGPIPE, SIG_IGN );
#endif

        if(verbose) { std::cerr << "]" << std::endl; }
    }

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
            if( ::gethostname( *blk, blk.length() ) == SOCKET_ERROR ) {
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

