#include "y/net/io/engine.hpp"


namespace upsylon
{
    namespace net
    {
        engine:: ~engine() throw()
        {
            Y_NET_VERBOSE(std::cerr << "[network.engine.quit]" << std::endl);
        }

        engine:: engine() :
        sockset(),
        tcp_servers( ),
        tcp_clients( )
        {
            Y_NET_VERBOSE(std::cerr << "[network.engine.init]" << std::endl);
        }

        
        void engine:: start( tcp_server_protocol *srv )
        {
            assert(srv!=NULL);
            const tcp_server_proto proto(srv);
            sockset.insert( *proto );
            try
            {
                if(!tcp_servers.insert(proto))
                {
                    throw upsylon::exception("net.engine.start(unexpected multiple tcp server socket)");
                }
                srv->blocking(false);
            }
            catch(...)
            {
                tcp_servers.no(proto->uuid);
                sockset.remove(*proto);
                throw;
            }

        }


        bool engine:: cycle( socket_delay &d )
        {
            Y_NET_VERBOSE(std::cerr<<"[net.engine.cycle(" << d.wait_for() << ")]" << std::endl);

            //------------------------------------------------------------------
            //
            // read incoming with delay
            //
            //------------------------------------------------------------------
            size_t na      = sockset.incoming(d);

            //------------------------------------------------------------------
            //
            //
            // TCP
            //
            //
            //------------------------------------------------------------------
            if( (na>0) )
            {
                handle_tcp_servers(na);
                if(na>0)
                {
                    handle_tcp_clients_incoming(na);
                }
            }


            
            return false;
        }


       

    }
}
