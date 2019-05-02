#include "y/net/io/engine.hpp"


namespace upsylon
{
    namespace net
    {
        io_engine:: ~io_engine() throw()
        {
        }

        io_engine:: io_engine() : sockset()
        {
        }


        void io_engine:: start( tcp_server_protocol *srv )
        {
            const tcp_server_proto p( srv );

            //------------------------------------------------------------------
            //
            // try to insert in socket set
            //
            //------------------------------------------------------------------
            sockset.insert(*p);

            //------------------------------------------------------------------
            //
            // try to insert in database
            //
            //------------------------------------------------------------------
            try
            {
                srv->blocking(false);
                if(!tcp_servers.insert(p))
                {
                    const socket_address &a = **srv;
                    throw upsylon::exception("net::io_engine:start(tcp_server %s@%u) multiple socket!", a.text(), bswp( a.port ) );
                }
            }
            catch(...)
            {
                sockset.remove(*p);
                throw;
            }


        }
    }
}
