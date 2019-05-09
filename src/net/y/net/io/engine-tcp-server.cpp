#include "y/net/io/engine.hpp"


namespace upsylon
{
    namespace net
    {
        bool engine:: handle_tcp_servers( size_t &na )
        {

            size_t n = tcp_servers.size();
            for(tcp_server_iterator it=tcp_servers.begin(); n-- > 0; ++it )
            {
                tcp_server_protocol &srv = **it;
                if( sockset.is_readable(srv) )
                {
                    //----------------------------------------------------------
                    //
                    // start a new connection
                    //
                    //----------------------------------------------------------
                    --na;
                    Y_NET_VERBOSE(std::cerr << "server<" << srv->text() << "@" << bswp(srv->port) << "> accept a new connection " << std::endl);

                    //----------------------------------------------------------
                    // use accept() and record the socket_type
                    //----------------------------------------------------------
                    tcp_link link  = srv.accept();
                    link->blocking(false);
                    sockset.insert(*link);

                    //----------------------------------------------------------
                    // create a new client for the server
                    //----------------------------------------------------------
                    try
                    {
                        const tcp_client_proto  proto = srv.create(link);
                        if(! tcp_clients.insert(proto) )
                        {
                            throw upsylon::exception("net.engine.accept(unexpected multiple tcp server socket)");
                        }
                    }
                    catch(...)
                    {
                        sockset.remove( *link );
                        throw;
                    }
                }

            }

            return false;
        }
    }

}

