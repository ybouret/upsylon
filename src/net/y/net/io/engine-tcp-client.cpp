
#include "y/net/io/engine.hpp"


namespace upsylon
{
    namespace net
    {
        void io_engine:: start( tcp_client_protocol *cln )
        {
            assert(cln);
            const tcp_client_proto p( cln );

            //------------------------------------------------------------------
            //
            // try to insert in socket set
            //
            //------------------------------------------------------------------
            tcp_client &ref = **cln;
            sockset.insert(ref);

            //------------------------------------------------------------------
            //
            // try to insert in database
            //
            //------------------------------------------------------------------
            try
            {
                ref.blocking(false);
                if( !tcp_clients.insert(p) )
                {
                    const socket_address &a = *ref;
                    throw upsylon::exception("net::io_engine:start(tcp_client %s@%u) multiple socket!", a.text(), bswp( a.port ) );
                }

            }
            catch(...)
            {
                sockset.remove(**p);
                throw;
            }

        }

        void io_engine:: check_tcp_clients(size_t &na)
        {
            size_t              n  = tcp_clients.size();
            tcp_client_iterator it = tcp_clients.begin();
            while( (na>0) && (n-->0) )
            {
                tcp_client_protocol &proto = **it;
                tcp_client          &cln   = *proto;
                
                if( sockset.is_readable(cln) )
                {
                    // load data
                    char buff[256];
                    size_t nr = cln.recv(buff,sizeof(buff));
                    std::cerr << "#recv=" << nr << std::endl;
                    --na;
                }
            }
        }

    }

}

