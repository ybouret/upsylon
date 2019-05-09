
#include "y/net/tcp/stream.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(tcp_client)
{
    if(argc<=2) throw exception("%s: version=[v4|v6] dest@port [msg...]",argv[0]);
    network::verbose = true;

    net::ip_version version = net::v4;
    {
        const string vstr = argv[1];
        if( vstr == "v4" )
        {
            // do nothing
        }
        else if( vstr == "v6" )
        {
            version = net::v6;
        }
        else
        {
            throw exception("invalid version='%s'", *vstr );
        }
    }

    net::tcp_link   client = new net::tcp_client(argv[2],version);
    net::tcp_cache  cache  = new net::tcp_recv_queue(32);
    std::cerr << "cache.block_size=" << cache->block_size << std::endl;

    std::cerr << "TCP->" << (*client)->text() << "@" << net::bswp( (*client)->port) << std::endl;

    for(int i=3;i<argc;++i)
    {
        string msg = argv[i];
        msg += "\r\n";
        const size_t ns = client->send_block(msg);
        if(ns<msg.size())
        {
            std::cerr << "couldn't send all message" << std::endl;
            break;
        }


    }

}
Y_UTEST_DONE()

