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
            }
            catch(...)
            {
                sockset.remove(*proto);
                throw;
            }

        }


        bool engine:: cycle( socket_delay &d )
        {

            //------------------------------------------------------------------
            //
            // read incoming
            //
            //------------------------------------------------------------------
            size_t na = sockset.incoming(d);
            if( na )
            {
                return true;
            }

            
            return false;
        }

    }
}
