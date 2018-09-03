#include "y/chem/species.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Chemical;

Y_UTEST(species)
{
    Species proton(  "H+",   1,0 );
    Species hydroxy( "HO-", -1,0  );
    
}
Y_UTEST_DONE()

