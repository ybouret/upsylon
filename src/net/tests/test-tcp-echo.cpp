#include "y/net/tcp/stream.hpp"
#include "y/net/io/tcp-send-queue.hpp"
#include "y/code/utils.hpp"
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
    std::cerr << "TCP->" << (**client) << std::endl;

    std::cerr << "\t<send>" << std::endl;
    {
        net::tcp_send_cache ocache = new net::tcp_send_queue(16);
        net::tcp_ostream    out( client, ocache );
        for(int i=3;i<argc;++i)
        {
            const char  *buf = argv[i];
            const size_t len = length_of(buf);
            for(size_t j=0;j<len;++j)
            {
                out << visible_char[uint8_t(buf[j])];
            }
            out << "\r\n";
        }
        out.flush();
    }
    std::cerr << "\t<send/>" << std::endl << std::endl;


    for(int i=3;i<argc;++i)
    {
        net::tcp_recv_cache icache = new net::tcp_recv_queue(32);
        net::tcp_istream    inp( client, icache );
        string line;
        if(!inp.gets(line))
        {
            throw exception("Missing Echo Line!");
        }
        std::cerr << line << std::endl;
    }


    
}
Y_UTEST_DONE()

