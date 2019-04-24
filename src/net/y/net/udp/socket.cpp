
#include "y/net/udp/socket.hpp"


namespace upsylon
{
    namespace net
    {

        udp_socket:: ~udp_socket() throw()
        {
        }

        udp_socket:: udp_socket( const ip_version version ) :
        bsd_socket(udp,version)
        {
        }

        void udp_socket:: sendto( const socket_address &peer, const void *data, const size_t size, const int flags)
        {
            Y_GIANT_LOCK();
            assert( !(data==NULL&size>0) );
            const int msgSize = int(size);
            if(msgSize<0)
            {
                throw upsylon::exception("sendto(%s): message size overflow", peer.text() );
            }

            const int msgSent = ::sendto(sock, (const char *)data, msgSize, flags,
                                         static_cast<const sockaddr *>(peer.ro()),
                                         unsigned( peer.length()) );
            if(msgSent!=msgSize)
            {
                throw net::exception( get_last_error_code(), "sendto(%s)", peer.text() );
            }
        }

    }
}

