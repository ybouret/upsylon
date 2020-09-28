
#include "y/aqua/library.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Aqua;

Y_UTEST(library)
{

    Library lib;

    Species &h = lib("H+",1);
    Species &w = lib("HO-",-1);

    (void)h;
    (void)w;
    std::cerr << lib << std::endl;

}
Y_UTEST_DONE()

