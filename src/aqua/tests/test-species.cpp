#include "y/aqua/species.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Aqua;

Y_UTEST(species)
{
    Species::Pointer h = new Species("H+",1);
    Species::Pointer w = new Species("HO-",-1);
    std::cerr << h->name << std::endl;
    std::cerr << w->name << std::endl;
    h->data.make<double>();
}
Y_UTEST_DONE()

