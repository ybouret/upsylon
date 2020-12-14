#include "y/net/comm/engine.hpp"

namespace upsylon
{
    namespace net
    {
        
        comm_engine:: ~comm_engine() throw()
        {
        }
        
        comm_engine:: comm_engine()   :
        sockset(),
        tcp_servers()
        {
        }
        
        void comm_engine:: start(comm_tcp_server *srv)
        {
            assert(srv);
            const comm_tcp_server::proto proto(srv);
            if( !tcp_servers.insert(proto) )
            {
                throw upsylon::exception("multiple TCP server!");
            }
            try
            {
                sockset.insert( *srv );
            }
            catch(...)
            {

            }
        }
    }
}
