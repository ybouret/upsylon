
#include "y/alchemy/parser.hpp"
#include "y/alchemy/reactor.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Alchemy;

Y_UTEST(parser)
{

    Library          lib;
    ConstEquilibrium eqn("equilib",1.1);

    std::cerr << eqn << std::endl;

    for(int i=1;i<argc;++i)
    {
        eqn.parse(argv[i],lib);
    }
    
    std::cerr << eqn << std::endl;

}
Y_UTEST_DONE()

