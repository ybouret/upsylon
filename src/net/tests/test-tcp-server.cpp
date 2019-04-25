#include "y/net/tcp/server.hpp"
#include "y/utest/run.hpp"
#include "y/string/convert.hpp"

using namespace upsylon;

Y_UTEST(tcp_server)
{
    if(argc<=2) throw exception("%s: version=[v4|v6] port",argv[0]);

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

    const uint16_t  user_port = uint16_t(string_convert::to<size_t>(argv[2],"port"));
    const unsigned  pending   = 1;
    net::tcp_server server( user_port, pending, version );
    
}
Y_UTEST_DONE()

