#include "y/alchemy/species.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Alchemy;

Y_UTEST(species)
{
    size_t indx=0;
    Species h("H+",1,++indx);
    Species w("HO-",-1,++indx);
    std::cerr << h << ", " << w << std::endl;
    

}
Y_UTEST_DONE()

