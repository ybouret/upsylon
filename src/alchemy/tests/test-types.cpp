#include "y/alchemy/library.hpp"
#include "y/alchemy/equilibrium.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"


using namespace upsylon;
using namespace Alchemy;


Y_UTEST(types)
{
    Y_UTEST_SIZEOF(Species);
    Y_UTEST_SIZEOF(Species::Pointer);
    Y_UTEST_SIZEOF(Species::Set);

    Y_UTEST_SIZEOF(Library);

}
Y_UTEST_DONE()

