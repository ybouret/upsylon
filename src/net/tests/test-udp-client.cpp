#include "y/net/udp/client.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(udp_client)
{
    if(argc<=2) throw exception("%s: version=[v4|v6] dest:port [msg]",argv[0]);

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

    const net::socket_addr_ex sax(argv[2],version);
    std::cerr << "UDP->" << sax->text() << "@" << net::bswp(sax->port) << std::endl;



}
Y_UTEST_DONE()


