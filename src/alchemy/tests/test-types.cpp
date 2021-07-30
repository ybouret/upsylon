#include "y/alchemy/library.hpp"
#include "y/alchemy/equilibrium.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"


using namespace upsylon;
using namespace alchemy;

namespace
{
    size_t len_for(unsigned long l)
    {
        if(l<=9)
        {
            return 1;
        }
        else
        {
            size_t n = 1;
            while( (l/=10) > 0 ) ++n;
            return n;
        }
    }
}

Y_UTEST(types)
{
    Y_UTEST_SIZEOF(species);
    Y_UTEST_SIZEOF(species::pointer);
    Y_UTEST_SIZEOF(library);
    Y_UTEST_SIZEOF(actor);
    Y_UTEST_SIZEOF(actors);
    Y_UTEST_SIZEOF(equilibrium);

    for(int i=1;i<argc;++i)
    {
        const unsigned long l = atol(argv[i]);
        std::cerr << l << " => " << len_for(l) << std::endl;
        const string s = vformat("%lu",l);
    }
}
Y_UTEST_DONE()

