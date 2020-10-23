

#include "y/aqua/engine.hpp"
#include "y/aqua/boot.hpp"
#include "y/aqua/library.hpp"
#include "y/utest/run.hpp"
#include "y/string/convert.hpp"

using namespace upsylon;
using namespace Aqua;

Y_UTEST(boot)
{
    double C0 = 1e-4;
    if(argc>1)
    {
        C0 = string_convert::to<double>(argv[1],"C0");
    }

    Library lib;

    Species &h   = lib("H+",1);
    Species &w   = lib("HO-",-1);
    Species &ah  = lib("AH",0);
    Species &am  = lib("Am",-1);
    Species &NH4 = lib("NH4+",1);
    Species &NH3 = lib("NH3",0);

    Species &Na = lib("Na+",1);  (void)Na;
    Species &Cl = lib("Cl-",-1); (void)Cl;

    lib.init();
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
    

    Boot boot("main");
    boot.electroneutrality(lib);
    boot.conserve(C0,ah,am);
    boot.conserve(0.000,NH4,NH3);
    boot.conserve(0,Na);
    boot.conserve(0.000,Cl);

    std::cerr << "boot=" << boot << std::endl;

    boot.verbose = true;

    boot.init(lib,eqs);
    
    
    vector<double> C(lib.entries()+2,0);
    lib.show(std::cerr,C);

    Engine engine;
    engine.init(lib,eqs);
    engine.computeK(0);

    boot.find(C,engine);
    


}
Y_UTEST_DONE()


