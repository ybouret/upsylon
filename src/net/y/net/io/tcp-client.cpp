
#include "y/net/io/tcp-client.hpp"


namespace upsylon
{
    namespace net
    {
        tcp_client_protocol:: ~tcp_client_protocol() throw() {}

        tcp_client_protocol:: tcp_client_protocol( const tcp_link &link ) throw() :
        tcp_link(link)
        {
        }
        

    }

}

