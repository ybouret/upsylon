
#include "y/chem/boot.hpp"
#include "y/chem/equilibria.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"

using namespace upsylon;
using namespace Chemical;

Y_UTEST(boot)
{

    Library   lib;
    Species  &proton  = lib("H+",  1);
    Species  &hydroxy = lib("HO-",-1);
    Species  &AH      = lib("AH",0);
    Species  &Am      = lib("Am",-1);
    Species  &NH3     = lib("NH3",0);
    Species  &NH4     = lib("NH4+",1);
    Species  &Na      = lib("Na+",1);
    Species  &Cl      = lib("Cl-",-1);

    Equilibria cs;

    {
        Equilibrium &water = cs("water",1e-14);
        water.add(proton);
        water.add(hydroxy);
    }

    {
        Equilibrium &acetic = cs("acetic",square_of(pow(10.0,-4.8)));
        acetic.add(proton,2);
        acetic.add(Am,2);
        acetic.add(AH,-2);
    }

    {
        Equilibrium &ammoniac = cs("ammoniac",(pow(10.0,-9.2)));
        ammoniac.add(proton);
        ammoniac.add(NH3);
        ammoniac.sub(NH4);
    }

    cs.compile_for(lib);
    

    Boot::Loader loader;
    loader.electroneutrality(lib);
    loader.conserve(0,Na);
    loader.conserve(0.1,Cl);
    loader.conserve(0.000001,Am,AH);
    loader.conserve(0.1,NH4,NH3);

    vector<double> C( cs.M+2 );
    lib.display(std::cerr,C,"\t(0) " );
    std::cerr << loader << std::endl;

    cs.computeK(0.0);
    std::cerr << "Nu=" << cs.Nu << std::endl;
    cs.boot(C,loader);

}
Y_UTEST_DONE()
