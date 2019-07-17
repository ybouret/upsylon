#include "y/lang/pattern/matching.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;

Y_UTEST(matching)
{
    if(argc>1)
    {
        Lang::Matching match(argv[1]);
        string         line;
        ios::icstream  fp( ios::cstdin );
        vector<Lang::Token> tokens;
        while( (std::cerr << "> ").flush(), fp.gets(line) )
        {

            if(match.exactly(line))
            {
                std::cerr << "+exact: " << match << std::endl;
            }
            else
            {
                std::cerr << "-exact" << std::endl;
            }

            if(match.partly(line))
            {
                std::cerr << "+partly: " << match << std::endl;
            }
            else
            {
                std::cerr << "-partly" << std::endl;
            }

            const size_t n = match.find(tokens,line);
            std::cerr << "all#" << n << "=" << tokens << std::endl;

            const Lang::Token *ptkn = match.first_in(line);
            if(ptkn)
            {
                std::cerr << "first_in=" << *ptkn << std::endl;
            }
            else
            {
                std::cerr << "no first_in" << std::endl;
            }
        }
    }
}
Y_UTEST_DONE()

