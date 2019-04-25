
#include "y/net/tcp/client.hpp"
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

    net::tcp_client client(argv[2],version);

    std::cerr << "TCP->" << client->text() << "@" << net::bswp(client->port) << std::endl;

    for(int i=3;i<argc;++i)
    {


    }

}
Y_UTEST_DONE()

