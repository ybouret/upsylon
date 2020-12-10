#include "y/net/comm/tcp-client.hpp"
#include "y/net/comm/tcp-server.hpp"

#include "y/utest/run.hpp"
#include "y/string/convert.hpp"

using namespace upsylon;
using namespace net;

namespace
{
    class TCP_Echo_Server : public comm_tcp_server
    {
    public:
        inline explicit TCP_Echo_Server(const socket_address &ip) : comm_tcp_server(ip,4)
        {
        }

        inline virtual ~TCP_Echo_Server() throw()
        {
            
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(TCP_Echo_Server);
    };

    
}

Y_UTEST(comm_proto)
{

    if(argc<=2) throw upsylon::exception("%s: version=[v4|v6] port",argv[0]);

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
            throw upsylon::exception("invalid version='%s'", *vstr );
        }
    }


    const uint16_t                 user_port = uint16_t(string_convert::to<size_t>(argv[2],"port"));
    const socket_addr_ex           server_ip(ip_addr_any,version,user_port);

    comm_tcp_server::proto server = new TCP_Echo_Server(*server_ip);

    

}
Y_UTEST_DONE()

