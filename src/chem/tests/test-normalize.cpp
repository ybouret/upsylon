
#include "y/chem/equilibria.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"

using namespace upsylon;
using namespace Chemical;

Y_UTEST(normalize)
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

    cs.computeK(0);
    
    // solve from 0
    std::cerr << "Normalize from 0" << std::endl;
    if( !cs.normalize(C) )
    {
        std::cerr << "cannot start from 0" << std::endl;
    }
    else
    {
        lib.display(std::cerr,C," (0) ");
    }

    for(size_t iter=0;iter<100;++iter)
    {
        for(size_t j=cs.M;j>0;--j)
        {
            C[j] = alea.to<double>() * (alea.to<double>() - 0.3 );
        }
        std::cerr << "Start from: " << std::endl;
        lib.display(std::cerr,C," (-) ");
        if( cs.normalize(C) )
        {
            std::cerr << "...normalized:" << std::endl;
            lib.display(std::cerr,C," (+) ");
            cs.computeGamma(C);
            std::cerr << "\t=> Gamma=" << cs.Gamma << std::endl;
        }

    }


}
Y_UTEST_DONE()
