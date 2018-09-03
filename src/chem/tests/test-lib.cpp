
#include "y/chem/library.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Chemical;

Y_UTEST(lib)
{
    Library lib;
    Species &proton  = lib("H+",1);
    Species &hydroxy = lib("HO-",-1);
    (void)proton;
    (void)hydroxy;
    lib("AH",0);
    lib("A-",-1);
    
    
}
Y_UTEST_DONE()

