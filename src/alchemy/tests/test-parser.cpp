
#include "y/alchemy/parser.hpp"
#include "y/alchemy/reactor.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Alchemy;

Y_UTEST(parser)
{

    Library          lib;
    ConstEquilibrium eqn("toto",1.1);

    std::cerr << eqn << std::endl;

    for(int i=1;i<argc;++i)
    {
        eqn.parse(argv[i],lib);
    }

}
Y_UTEST_DONE()

