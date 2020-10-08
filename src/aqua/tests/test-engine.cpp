

#include "y/aqua/engine.hpp"
#include "y/utest/run.hpp"
#include "y/mkl/kernel/quark.hpp"

using namespace upsylon;
using namespace Aqua;

Y_UTEST(engine)
{
    Library lib;

    Species &h   = lib("H+",1);
    Species &w   = lib("HO-",-1);
    Species &ah  = lib("AH",0);
    Species &am  = lib("Am",-1);
    Species &NH4 = lib("NH4+",1);
    Species &NH3 = lib("NH3",0);

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

    {
        Equilibrium &ammoniac = eqs.constant("ammoniac",pow(10.0,-9.2));
        ammoniac(NH4,-1);
        ammoniac(NH3,1);
        ammoniac(h,1);
    }



    std::cerr << eqs << std::endl;

    Engine cs;
    cs.init(lib,eqs);
    std::cerr << "Nu      = " << cs.Nu      << std::endl;
    std::cerr << "Nu2     = " << cs.Nu2     << std::endl;
    std::cerr << "dNu2    = " << cs.det     << std::endl;
    std::cerr << "active  = " << cs.active  << std::endl;
    std::cerr << "Ma      = " << cs.Ma      << " #/ " << cs.M << std::endl;
    std::cerr << "Nc      = " << cs.Nc      << std::endl;


}
Y_UTEST_DONE()

