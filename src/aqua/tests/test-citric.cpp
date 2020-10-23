

#include "y/aqua/engine.hpp"
#include "y/aqua/boot.hpp"

#include "y/utest/run.hpp"
#include "y/string/convert.hpp"
#include "y/mkl/tao.hpp"

using namespace upsylon;
using namespace Aqua;

Y_UTEST(citric)
{
    double C0  = 0;
    if(argc>1)
    {
        C0 = string_convert::to<double>(argv[1],"C0");
    }

    Library lib;

    Species &h   = lib("H+",1);
    Species &w   = lib("HO-",-1);
    Species &Na  = lib("Na+",1); (void) Na;
    Species &Cl  = lib("Cl-",1); (void) Cl;
    Species &NH4 = lib("NH4+",1);
    Species &NH3 = lib("NH3",0);


    Equilibria eqs;
    eqs.solvant("water", h, w, 1e-14);
    eqs.weakAcid("ammoniac", NH4, NH3, h, pow(10.0,-9.2));

    Species &ahhh = lib("ahhh", 0);
    Species &ahhm = lib("ahhm",-1);
    Species &ahmm = lib("ahmm",-2);
    Species &ammm = lib("ammm",-3);

    eqs.weakAcid("E1", ahhh, ahhm, h, pow(10.0,-3.13));
    eqs.weakAcid("E2", ahhm, ahmm, h, pow(10.0,-4.76));
    eqs.weakAcid("E3", ahmm, ammm, h, pow(10.0,-6.40));

    std::cerr << lib << std::endl;
    std::cerr << eqs << std::endl;

    Boot boot("main");
    boot.electroneutrality(lib);
    boot.conserve(C0, ahhh, ahhm, ahmm, ammm);
    boot.conserve(0,Na);
    boot.conserve(0,Cl);
    boot.conserve(0,NH4,NH3);
    

    std::cerr << boot << std::endl;


    Engine engine;
    lib.init();
    engine.init(lib,eqs);
    engine.computeK(0);

    boot.verbose = true;
    boot.init(lib,eqs);

    vector<double> C(engine.M,0);
    boot.find(C,engine);


}
Y_UTEST_DONE()

