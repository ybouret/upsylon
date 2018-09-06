
#include "y/chem/equilibria.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/math/kernel/tao.hpp"

using namespace upsylon;
using namespace Chemical;

Y_UTEST(balance)
{

    Library   lib;
    Species  &proton  = lib("H+",  1);
    Species  &hydroxy = lib("HO-",-1);
    Species  &AH      = lib("AH",0);
    Species  &Am      = lib("Am",-1);
    Species  &NH3     = lib("NH3",0);
    Species  &NH4     = lib("NH4+",1);
    lib("Na+",1);
    lib("Cl-",-1);

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
    vector<double> C(cs.M+2);

    C.ld(0);

    C[1] =  0.0;
    C[2] =  0;
    C[3] =  0.11;
    C[4] =  -0.1;
    //C[4] =  0.301;

    /*
    for(size_t i=C.size();i>0;--i)
    {
        C[i] = alea.symm<double>();
    }
     */

    lib.display(std::cerr,C," (-) ");
    std::cerr << "Nu    =" << cs.Nu  << std::endl;
    std::cerr << "nu2   =" << cs.nu2 << std::endl;
    cs.computeK(0.0);
    if(cs.balance(C))
    {
        std::cerr << "balanced=" << C << std::endl;
        if(cs.tryShift(C))
        {
            std::cerr << "shifted=" << C << std::endl;
        }
        else
        {
            std::cerr << "unable to shift" << std::endl;
        }
    }
    else
    {
        std::cerr << "unable to balance" << std::endl;
    }

}
Y_UTEST_DONE()

