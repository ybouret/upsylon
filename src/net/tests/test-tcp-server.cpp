#include "y/net/tcp/server.hpp"
#include "y/utest/run.hpp"
#include "y/string/convert.hpp"
#include "y/net/tcp/stream.hpp"
#include "y/sequence/vector.hpp"
#include "y/string/tokenizer.hpp"

using namespace upsylon;

static inline bool isSep( const int C ) { return C == ' ' || C == '\t'; }

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


    const uint16_t                 user_port = uint16_t(string_convert::to<size_t>(argv[2],"port"));
    const unsigned                 pending   = 1;
    net::tcp_server                server( user_port, pending, version );
    vector<string>                 words(16,as_capacity);
    net::tcp_recv_cache            icache = new net::tcp_recv_queue(32);
    net::tcp_send_cache            ocache = new net::tcp_send_queue(32);

    std::cerr << "cache.block_size=" << icache->block_size << std::endl;

    while(true)
    {
        net::tcp_link client = server.accept();
        std::cerr << "Connexion From " << (*client)->text() << std::endl;

        net::tcp_istream fp( client, icache );
        net::tcp_ostream op( client, ocache );

        string line;
        while( fp.gets(line) )
        {
            tokenizer<char>::split(words, line, isSep );
            if(words.size()<=0) continue;
            std::cerr << words << std::endl;
            for(size_t i=1;i<=words.size();++i)
            {
                op << '<' << words[i] << '>' << '\n';
            }
            op.flush();
            if( "stop" == words[1] )
            {
                return 0;
            }
        }
        

    }
    
}
Y_UTEST_DONE()

