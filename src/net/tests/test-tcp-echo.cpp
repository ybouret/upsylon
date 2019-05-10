#include "y/net/tcp/stream.hpp"
#include "y/net/io/tcp-send-queue.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(tcp_echo)
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

    net::tcp_link       client = new net::tcp_client(argv[2],version);
    net::tcp_cache      icache = new net::tcp_recv_queue(32);
    net::tcp_istream    inp( client, icache );

    net::tcp_send_queue ocache(16);

    std::cerr << "TCP->" << (*client)->text() << "@" << net::bswp( (*client)->port) << std::endl;

    for(int i=3;i<argc;++i)
    {
        string msg = argv[i];
        msg += "\r\n";
        ocache.push(msg);
        std::cerr << "ocache=" << ocache << std::endl;
    }

    
    std::cerr << "ocache.size=" << ocache.size() << std::endl;
    while( ocache.size() )
    {
        const size_t ns = ocache.comm(*client);
        if( ns <= 0 )
        {
            break;
        }
        std::cerr << "ocache=" << ocache << std::endl;
    }

    string line;
    for(int i=3;i<argc;++i)
    {
        if( !inp.gets(line) ) break;
        std::cerr << line << std::endl;
    }

}
Y_UTEST_DONE()

