#include "y/net/tcp/server.hpp"
#include "y/utest/run.hpp"
#include "y/string/convert.hpp"
#include "y/net/tcp/ostream.hpp"
#include "y/net/tcp/istream.hpp"
#include "y/net/comm/cache.hpp"
#include "y/sequence/vector.hpp"
#include "y/string/tokenizer.hpp"

using namespace upsylon;

static inline bool isSep( const int C ) { return C == ' ' || C == '\t'; }

Y_UTEST(tcp_server)
{

    if(argc<=2) throw exception("%s: version=[v4|v6] port",argv[0]);

    network::verbose        = true;
    net::ip_version version =  network::ip_version(argv[1]);
    net::comm_cache &cache  = net::comm_cache::instance();

    const uint16_t                 user_port = uint16_t(string_convert::to<size_t>(argv[2],"port"));
    const unsigned                 pending   = 1;
    net::tcp_server                server( user_port, pending, version );
    vector<string>                 words(16,as_capacity);

    
    while(true)
    {
        net::tcp_link client = server.accept();
        std::cerr << "Connexion From " << (**client)<< std::endl;

        net::tcp_istream fp( client, cache.query(5) );
        net::tcp_ostream op( client, cache.query(5) );

        string line;
        while( fp.gets(line) )
        {
            tokenizer<char>::split(words, line, isSep );
            if(words.size()<=0) continue;
            std::cerr << "recv=" << words << std::endl;
            std::cerr << "    |_echoing: init..." << std::endl;
            for(size_t i=1;i<=words.size();++i)
            {
                op << '<' << words[i] << '>';
            }
            op << '\n';
            op.flush();
            std::cerr << "    |_echoing: done" << std::endl;
            if( "stop" == words[1] )
            {
                return 0;
            }
        }
        

    }

}
Y_UTEST_DONE()

