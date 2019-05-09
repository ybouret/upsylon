
#include "y/net/io/engine.hpp"


namespace upsylon
{
    namespace net
    {
        bool engine:: handle_tcp_clients_incoming( size_t &na )
        {
            size_t n = tcp_clients.size();
            for(tcp_client_iterator it=tcp_clients.begin(); n-- > 0; ++it )
            {
                tcp_client_protocol &client = **it;
                const bsd_socket    &s      = *client;
                if( sockset.is_readable(s) )
                {
                    --na;
                    
                }
            }
            return false;
        }

    }

}
