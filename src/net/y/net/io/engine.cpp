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

        bool io_engine:: cycle( socket_delay &d )
        {
            size_t na =  sockset.probe(d);
            if(na>0)
            {
                //--------------------------------------------------------------
                //
                // probing tcp servers
                //
                //--------------------------------------------------------------
                {
                    size_t n = tcp_servers.size();
                    tcp_server_iterator it = tcp_servers.begin();
                    while(n-->0)
                    {
                        tcp_server_protocol  &srv = **it;
                        const socket_address &sip = *srv;
                        if( sockset.is_readable(srv) )
                        {
                            // new connection ?
                            std::cerr << "New connection on " << sip.text() << "@" << bswp( sip.port ) << std::endl;
                        }
                        ++it;
                    }
                }


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
