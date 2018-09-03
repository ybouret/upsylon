#include "y/chem/reactions.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Chemical;

Y_UTEST(cs)
{

    Library   lib;
    Species  &proton  = lib("H+",1);
    Species  &hydroxy = lib("HO-",1);
    Species  &AH      = lib("AH",0);
    Species  &Am      = lib("Am",-1);

    Reactions cs;

    {
        Reaction &water = cs("water",1e-14);
        water.add(proton);
        water.add(hydroxy);
    }

    {
        Reaction &acetic = cs("acetic",pow(10.0,-4.8));
        acetic.add(proton);
        acetic.add(Am);
        acetic.sub(AH);
    }

    std::cerr << cs << std::endl;
}
Y_UTEST_DONE()


