#include "y/net/tcp/ostream.hpp"
#include "y/net/tcp/istream.hpp"
#include "y/net/comm/cache.hpp"
#include "y/code/utils.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(tcp_echo)
{
    if(argc<=2) throw exception("%s: version=[v4|v6] dest@port [msg...]",argv[0]);
    network::verbose       = true;
    net::comm_cache &cache = net::comm_cache::instance();

    net::ip_version version = network::ip_version(argv[1]);
    net::tcp_link   client  = new net::tcp_client(argv[2],version);
    std::cerr << "TCP->" << (**client) << std::endl;

    std::cerr << "\t<send>" << std::endl;
    {
        net::tcp_ostream    out( client, cache.query(5)  );
        for(int i=3;i<argc;++i)
        {
            const char  *buf = argv[i];
            const size_t len = length_of(buf);
            for(size_t j=0;j<len;++j)
            {
                out << cchars::visible[uint8_t(buf[j])];
            }
            out << "\r\n";
        }
        out.flush();
    }
    std::cerr << "\t<send/>" << std::endl << std::endl;


    for(int i=3;i<argc;++i)
    {
        net::tcp_istream inp( client, cache.query(5) );
        string line;
        if(!inp.gets(line))
        {
            throw exception("Missing Echo Line!");
        }
        std::cerr << line << std::endl;
    }


    
}
Y_UTEST_DONE()

