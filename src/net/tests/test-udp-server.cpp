
#include "y/net/udp/server.hpp"
#include "y/utest/run.hpp"
#include "y/string/convert.hpp"

using namespace upsylon;

Y_UTEST(udp_server)
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

    const uint16_t user_port = uint16_t(string_convert::to<size_t>(argv[2],"port"));

    net::udp_server server( user_port, version );

    static const size_t MAX_LENGTH = 255;
    char buff[MAX_LENGTH+1];

    while(true)
    {
        memset(buff,0,sizeof(buff));
        const size_t nr = server.recv(buff,MAX_LENGTH);
        std::cerr << "nr=" << nr << " from " << *(server.peer) << std::endl;
        string msg(buff,nr);
        std::cerr << "|_'" << msg << "'" << std::endl;
        msg.reverse();
        server.send(msg);
        if(msg=="pots") break;
    }
}
Y_UTEST_DONE()

