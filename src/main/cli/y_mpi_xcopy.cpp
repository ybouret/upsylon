#include "y/program.hpp"
#include "y/ios/icstream.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/string/tokenizer.hpp"
#include "y/sort/unique.hpp"
#include "y/fs/vfs.hpp"
#include <cstdlib>

using namespace upsylon;


Y_PROGRAM_START()
{
    if(argc<=1)
    {
        throw exception("usage: %s hostfile [binary]",program);
    }
    
    // parsing hosts
    vector<string,memory::pooled> hosts(16,as_capacity);
    {
        const string  hostfile = argv[1];
        ios::icstream fp(hostfile);
        string        line;
        while( fp.gets(line) )
        {
            if( line.size() <= 0 ) continue;
            if( line[0] == '#' )   continue;
            tokenizer<char> tkn(line);
            if( tkn.next_with(" \t") )
            {
                const string id( tkn.token(), tkn.units() );
                hosts.push_back(id);
            }
        }
    }
    unique(hosts);
    
    if(argc>2)
    {
        const string filename = argv[2];
        const string basename = vfs::get_base_name(filename);
        for(size_t i=1;i<=hosts.size();++i)
        {
            const string cmd      = "scp " + filename + " " + hosts[i] + ":" + basename;
            std::cerr << "executing <" << cmd << ">" << std::endl;
            if( 0 != system( *cmd ) )
            {
                throw exception("unable to [%s]", *cmd);
            }
        }
    }
    
    
}
Y_PROGRAM_END()


