#include "y/net/io/engine.hpp"


namespace upsylon
{
    namespace net
    {
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

        void io_engine:: check_tcp_servers( size_t &na )
        {
            size_t              n  = tcp_servers.size();
            tcp_server_iterator it = tcp_servers.begin();
            while( (na>0) && (n-->0) )
            {
                tcp_server_protocol  &srv = **it;
                const socket_address &sip = *srv;
                if( sockset.is_readable(srv) )
                {
                    Y_NET_VERBOSE(std::cerr << "[network.io_engine(tcp_client_proto: " << sip.text() << "@" << bswp( sip.port ) << ")]" << std::endl);
                    //__________________________________________________________
                    //
                    // low level call
                    //__________________________________________________________
                    const tcp_link link = srv.accept();

                    //__________________________________________________________
                    //
                    // high level call
                    //__________________________________________________________
                    start( srv.create(link) );

                    //__________________________________________________________
                    //
                    // update activities
                    //__________________________________________________________
                    --na;
                }
                ++it;
            }
        }


    }

}

