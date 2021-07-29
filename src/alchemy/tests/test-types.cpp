#include "y/alchemy/library.hpp"
#include "y/alchemy/actor.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"


using namespace upsylon;
using namespace alchemy;

Y_UTEST(types)
{
    Y_UTEST_SIZEOF(species);
    Y_UTEST_SIZEOF(species::pointer);
    Y_UTEST_SIZEOF(library);
    Y_UTEST_SIZEOF(actor);
}
Y_UTEST_DONE()

