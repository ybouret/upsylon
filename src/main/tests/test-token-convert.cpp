
#include "y/lang/pattern/matching.hpp"
#include "y/lang/token-convert.hpp"

#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;


Y_UTEST(token_convert)
{
    Lang::Matching iMatch = "-?[:digit:]+";
    Lang::Matching uMatch = "[:digit:]+";

    string input;
    for(int i=1;i<argc;++i)
    {
        if(i>1) input << ' ';
        input << argv[i];
    }
    std::cerr << "[" << input << "]" << std::endl;

    vector<Lang::Token> tokens;
    {
        const size_t ni = iMatch.find(tokens,input);
        std::cerr << "ni=" << ni << std::endl;
        std::cerr << "  |_" << tokens << std::endl;
        for(size_t i=1;i<=ni;++i)
        {
            std::cerr << "   |_<" << Lang::TokenConvert::ToUnit( tokens[i] ) << ">" << std::endl;
        }
    }

    {
        const size_t nu = uMatch.find(tokens,input);
        std::cerr << "nu=" << nu << std::endl;
        std::cerr << "  |_" << tokens << std::endl;
        for(size_t i=1;i<=nu;++i)
        {
            std::cerr << "   |_<" << Lang::TokenConvert::ToSize( tokens[i] ) << ">" << std::endl;
        }
    }

}
Y_UTEST_DONE()
