#include "y/jargon/pattern/regexp.hpp"
#include "y/jargon/pattern.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
using namespace upsylon;
using namespace Jargon;

Y_UTEST(matching)
{
    if(argc>1)
    {
        Motif    p(  RegularExpression::Compile(argv[1]) );
        Token    token;
        list<Token> tokens;
        if(argc>2)
        {
            ios::icstream fp( argv[2] );
            string        line;
            while( fp.gets(line) )
            {
                std::cerr << "'" << line << "'" << std::endl;
                std::cerr << "\texact:  ";
                if(p->exactlyMatchesData(token,line))
                {
                    std::cerr << token << std::endl;
                }
                else
                {
                    std::cerr << "[NO]" << std::endl;
                }
                std::cerr << "\tpartly: ";
                if(p->somehowMatchesData(token,line))
                {
                   std::cerr << token << std::endl;
                }
                else
                {
                    std::cerr << "[NO]" << std::endl;
                }
                tokens.free();
                std::cerr << "\tevery : ";
                if(p->findMatchesOfData(tokens,line))
                {
                    std::cerr << tokens;
                }
                std::cerr << std::endl;
            }
        }
    }
}
Y_UTEST_DONE()

