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
        sockset()
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
                    throw upsylon::exception("net.engine.start(unexpected multiple socket)");
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
            // read incoming
            //
            //------------------------------------------------------------------
            size_t na = sockset.incoming(d);
            if( na )
            {
                handle_tcp_servers(na);
                return true;
            }


            
            return false;
        }


        bool engine:: handle_tcp_servers( size_t &na )
        {

            size_t n = tcp_servers.size();
            for( tcp_server_iterator it=tcp_servers.begin(); n-- > 0; ++it )
            {
                tcp_server_protocol &srv = **it;
                if( sockset.is_readable(srv) )
                {
                    Y_NET_VERBOSE(std::cerr << "server<" << srv->text() << "@" << bswp(srv->port) << "> accept a new connection " << std::endl);
                    --na;
                }
            }

            return false;
        }

    }
}
