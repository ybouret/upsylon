
#include "y/aqua/solver.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Aqua;

Y_UTEST(solver)
{
    Library lib;

    Species &h  = lib("H+",1);
    Species &w  = lib("HO-",-1);
    Species &ah = lib("AH",0);
    Species &am = lib("Am",-1);
    Species &Na = lib("Na+",1);  (void)Na;
    Species &Cl = lib("Cl-",-1); (void)Cl;

    std::cerr << lib << std::endl;

    Equilibria eqs;
    {
        Equilibrium &water = eqs.constant("water",1e-14);
        water(h,1);
        water(w,1);
    }

    {
        Equilibrium &acid = eqs.constant("acid",pow(10.0,-4.8));
        acid(ah,-1);
        acid(am,1);
        acid(h,1);
    }

    std::cerr << eqs << std::endl;

    Solver cs;
    cs.init(lib,eqs);
    std::cerr << "nu=" << cs.nu << std::endl;

}
Y_UTEST_DONE()

