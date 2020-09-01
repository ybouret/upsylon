
#include "y/jive/pattern/matching.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"
#include "y/ios/icstream.hpp"
#include "y/fs/vfs.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;
using namespace Jive;

Y_UTEST(jive_match)
{

    if(argc>1)
    {
        Matching match = argv[1];
        std::cerr << "firstChars:" << match.firstChars << std::endl;
        
        if( argc>2 )
        {
            const string  fileName = argv[2];
            const string  baseName = vfs::get_base_name(fileName);
            ios::icstream fp(fileName);
            string        line;
            unsigned      iline=1;
            while( (std::cerr << "> ").flush(),fp.gets(line) )
            {
                const string dataName = baseName + vformat(":%u: ",iline);
                std::cerr << "---> " << line << std::endl;
                std::cerr << "|_exactly: " << (NULL!=match.isExactly(dataName,line)) << std::endl;
                std::cerr << "|_foundIn: ";
                const Token *token = match.isFoundIn(dataName,line);
                if(token)
                {
                    std::cerr  << "'" << *token << "'" << std::endl;
                }
                else
                {
                    std::cerr << "none" << std::endl;
                }
                vector<Token::Handle> tokens;
                const size_t n = match.collect(tokens,dataName,line);
                std::cerr << "|_collect: " << tokens << std::endl;
                std::cerr << " |_" << n << std::endl;
                ++iline;
            }
        }

    }

}
Y_UTEST_DONE()
