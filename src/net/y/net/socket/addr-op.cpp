#include "y/net/net.hpp"
#include <iostream>

#if defined(Y_BSD)
#include <cerrno>
#endif


namespace upsylon
{
    namespace net
    {

        void socket_address:: bind( socket_type &sock ) const
        {
            Y_NET_VERBOSE(std::cerr << "[network.bind(" << text() << '@' << bswp(port) <<  ")]" << std::endl);
            Y_GIANT_LOCK();

#if defined(Y_WIN)
            if( SOCKET_ERROR == ::bind( sock, static_cast<const sockaddr*>( ro() ), int( length() )))
            {
                throw net::exception( Y_NET_LAST_ERROR(), "::bind(%s)", text() );
            }
#endif

#if defined(Y_BSD)
            while( ::bind(sock,static_cast<const sockaddr*>( ro() ), unsigned( length()) ) < 0 )
            {
                const int err = errno;
                switch(err)
                {
                    case EINTR:
                        continue;

                    default:
                        throw  net::exception( err, "::bind(%s)", text() );
                }
            }
#endif

        }

        void socket_address:: connect( socket_type &sock ) const
        {
            Y_NET_VERBOSE(std::cerr << "[network.connect(" << text() << '@' << bswp(port) <<  ")]" << std::endl);
            Y_GIANT_LOCK();

#if defined(Y_WIN)
            if( SOCKET_ERROR == ::connect( sock, static_cast<const sockaddr*>( ro() ), int( length() )))
            {
                throw net::exception( Y_NET_LAST_ERROR(), "::connect(%s)", text() );
            }
#endif

#if defined(Y_BSD)
            while( ::connect(sock, static_cast<const sockaddr*>( ro() ), unsigned( length()) ) < 0 )
            {
                const int err = errno;
                switch(err)
                {
                    case EINTR:
                        continue;

                    default:
                        throw  net::exception( err, "::connect(%s)", text() );
                }
            }
#endif
        }

        

    }

}


