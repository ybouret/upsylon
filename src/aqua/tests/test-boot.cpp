

#include "y/aqua/engine.hpp"
#include "y/aqua/boot.hpp"
#include "y/aqua/library.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Aqua;

Y_UTEST(boot)
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
    

    Boot boot;
    boot.electroneutrality(lib);
    boot.conserve(1e-8,ah,am);
    boot.conserve(0.000,NH4,NH3);
    boot.conserve(0,Na);
    boot.conserve(0.000,Cl);

    std::cerr << "boot=" << boot << std::endl;


    boot.init(lib,eqs);
    
    std::cerr << "R="  << boot.R << std::endl;
    std::cerr << "L="  << boot.L << std::endl;
    std::cerr << "d="  << boot.d << std::endl;
    std::cerr << "S="  << boot.S << std::endl;
    std::cerr << "pS=" << boot.pS << std::endl;
    std::cerr << "dS=" << boot.dS << std::endl;

    vector<double> C(lib.entries()+2,0);
    lib.show(std::cerr,C);

    Engine engine;
    engine.init(lib,eqs);

    boot.find(C,engine);



}
Y_UTEST_DONE()


