#include "y/net/io/engine.hpp"


namespace upsylon
{
    namespace net
    {
        io_engine:: ~io_engine() throw()
        {
            Y_NET_VERBOSE(std::cerr << "[network.io_engine.quit]" << std::endl);
        }

        io_engine:: io_engine() :
        sockset(),
        tcp_servers(tcp_servers_init,as_capacity),
        tcp_clients(tcp_clients_init,as_capacity)
        {
            Y_NET_VERBOSE(std::cerr << "[network.io_engine.init]" << std::endl);
        }

        
    

        bool io_engine:: cycle( socket_delay &d )
        {
            Y_NET_VERBOSE(std::cerr << "[network.io_engine.cycle]" << std::endl);
            size_t na =  sockset.probe(d);
            if(na>0)
            {
                Y_NET_VERBOSE(std::cerr << "[network.io_engine.activities=" << na << "]" << std::endl);
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
