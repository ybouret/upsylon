#include "y/net/comm/tcp-client.hpp"
#include "y/net/comm/tcp-server.hpp"
#include "y/net/comm/engine.hpp"

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

    if(argc<=2)
    {
        std::cerr << "usage: " << program << " version=[v4|v6] port" << std::endl;
    }
    else
    {
        network::verbose = true;
        net::ip_version        version   = network::ip_version(argv[1]);
        const uint16_t         user_port = uint16_t(string_convert::to<size_t>(argv[2],"port"));
        const socket_addr_ex   server_ip(ip_addr_any,version,user_port);
        comm_engine            engine;
        engine.start( new TCP_Echo_Server(*server_ip) );

    }
    

}
Y_UTEST_DONE()

