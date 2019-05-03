#include "y/net/io/engine.hpp"


namespace upsylon
{
    namespace net
    {
        io_engine:: ~io_engine() throw()
        {
        }

        io_engine:: io_engine() :
        sockset(),
        tcp_servers(tcp_servers_init,as_capacity),
        tcp_clients(tcp_clients_init,as_capacity)
        {
        }

        
    

        bool io_engine:: cycle( socket_delay &d )
        {
            size_t na =  sockset.probe(d);
            if(na>0)
            {
                check_tcp_servers(na);
                check_tcp_clients(na);
                return true;
            }
            else
            {
                Y_NET_VERBOSE(std::cerr << "[network.io_engine.no_activity]" << std::endl);
                return false;
            }
        }

    }
}
