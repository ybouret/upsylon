

#include "y/aqua/equilibria.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Aqua;

Y_UTEST(eqs)
{
    Species::Pointer h  = new Species("H+",1);
    Species::Pointer w  = new Species("HO-",-1);
    Species::Pointer AH = new Species("AH",0);
    Species::Pointer Am = new Species("A-",-1);

    Equilibria eqs;

    {
        Equilibrium &water = eqs( new ConstEquilibrium("water", 1e-14) );
        water(*h,1);
        water(*w,1);
        std::cerr << water << std::endl;
        water.validate();
    }

    {
        Equilibrium &acid = eqs( new ConstEquilibrium("acid", pow(10,-4.8) ) );
        acid(*h,1);
        acid(*Am,1);
        acid(*AH,-1);

        std::cerr << acid  << std::endl;
        
        acid.validate();
    }

    std::cerr << eqs << std::endl;

}
Y_UTEST_DONE()

