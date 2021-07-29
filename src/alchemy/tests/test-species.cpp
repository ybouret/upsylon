#include "y/alchemy/species.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace alchemy;

Y_UTEST(species)
{
    species                h("H+",1);
    const string           OH = "HO-";
    const species::pointer pw = new species(OH,-1);
    const species          w(*pw);
    Y_CHECK(w.name==pw->name);
    std::cerr << h.name << ", " << pw->name << ", " << w.name << std::endl;
}
Y_UTEST_DONE()

