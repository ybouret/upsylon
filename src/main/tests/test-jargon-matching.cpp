#include "y/jargon/pattern/matching.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
using namespace upsylon;
using namespace Jargon;

Y_UTEST(jargon_matching)
{
    Cache cache;
    if(argc>1)
    {
        Matching match(argv[1]);
        Token    token(cache);
        
        if(argc>2)
        {
            ios::icstream fp( argv[2] );
            string        line;
            while( fp.gets(line) )
            {
                std::cerr << "exact:  ";
                if(match.exactly(token,line))
                {
                    std::cerr << token << std::endl;
                }
                else
                {
                    std::cerr << "[NO]" << std::endl;
                }
                std::cerr << "partly: ";
                if(match.partly(token,line))
                {
                   std::cerr << token << std::endl;
                }
                else
                {
                    std::cerr << "[NO]" << std::endl;
                }
                
            }
        }
    }
}
Y_UTEST_DONE()

