
#include "y/aqua/solver.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;
using namespace Aqua;

Y_UTEST(solver)
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

    Solver cs;
    cs.init(lib,eqs);
    std::cerr << "Nu      = " << cs.Nu      << std::endl;
    std::cerr << "Nu2     = " << cs.Nu2     << std::endl;
    std::cerr << "dNu2    = " << cs.dNu2    << std::endl;
    std::cerr << "active  = " << cs.active  << std::endl;
    std::cerr << "A       = " << cs.A       << " #/ " << cs.M << std::endl;
    std::cerr << "P       = " << cs.P       << std::endl;
    
    vector<double> C(cs.M+2,0);

    //cs.balanceVerbose = true;

    for(size_t iter=0;iter<10;++iter)
    {

        for(size_t j=C.size();j>0;--j)
        {
            C[j] = alea.symm<double>() * pow(10.0,-8*alea.to<double>());
        }

        lib.show(std::cerr << "ini=",C);


        if(cs.balance(C))
        {
            lib.show(std::cerr << "end=",C);

        }
        else
        {
            std::cerr << "couldnt' balance!" <<std::endl;
        }
    }

}
Y_UTEST_DONE()

