
#include "y/net/io/tcp-client.hpp"


namespace upsylon
{
    namespace net
    {
        tcp_client_protocol:: ~tcp_client_protocol() throw() {}

        tcp_client_protocol:: tcp_client_protocol( const tcp_link &l ) throw() :
        link(l)
        {
        }

        const socket_id_t & tcp_client_protocol:: key() const throw()
        {
            return link->uuid;
        }

        const bsd_socket  & tcp_client_protocol:: operator*() const throw()
        {
            return *link;
        }


    }

}

