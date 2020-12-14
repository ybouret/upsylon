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

            //------------------------------------------------------------------
            //
            // register new server in db
            //
            //------------------------------------------------------------------
            const comm_tcp_server::proto proto(srv);
            if( !tcp_servers.insert(proto) )
            {
                upsylon::exception excp("multiple TCP server@");
                (*srv)->xcat(excp);
                throw excp;
            }

            //------------------------------------------------------------------
            //
            // register server socket
            //
            //------------------------------------------------------------------
            try
            {
                sockset.insert( *srv );
            }
            catch(...)
            {
                (void) tcp_servers.remove(srv->key());
                throw;
            }
        }
    }
}
