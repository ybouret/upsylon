
#include "y/aqua/solver.hpp"
#include "y/aqua/boot.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/mkl/kernel/quark.hpp"

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
    std::cerr << "dNu2    = " << cs.det     << std::endl;
    std::cerr << "active  = " << cs.active  << std::endl;
    std::cerr << "A       = " << cs.A       << " #/ " << cs.M << std::endl;
    std::cerr << "Nc      = " << cs.Nc      << std::endl;
    
    vector<double> C(cs.M+2,0);

    //cs.balanceVerbose = true;
    //cs.forwardVerbose = true;

    for(size_t iter=0;iter<10;++iter)
    {
        while(true)
        {
            for(size_t j=C.size();j>0;--j)
            {
                C[j] = alea.symm<double>() * pow(10.0,-8*alea.to<double>());
            }

            lib.show(std::cerr << "ini=",C);

            if(cs.balance(C))
            {
                lib.show(std::cerr << "balance=",C);
                std::cerr << "in #balanceCycle=" << cs.lastBalanceCycles << std::endl;
                break;
            }
            else
            {
                std::cerr << "couldnt' balance!" <<std::endl;
            }
        }


        cs.computeK(0);
        std::cerr << "K=" << cs.K <<  std::endl;
        if(cs.forward(C))
        {
            lib.show(std::cerr << "forward=",C);
            std::cerr << "in #forwardCycle=" << cs.lastForwardCycles << std::endl;
        }
        else
        {
            std::cerr << "couldn't forward!" << std::endl;
        }
    }

    {

        for(size_t j=C.size();j>0;--j)
        {
            C[j] = alea.to<double>() * pow(10.0,-8*alea.to<double>());
        }
        lib.show(std::cerr << "SWEEP=",C);

        for(size_t i=1;i<=cs.N;++i)
        {
            cs.equilibria[i]->sweep(cs.K[i],C,*cs.Caux,true);
        }

        mkl::quark::ld(C,0);
        cs.forwardVerbose = true;

        lib.show(std::cerr << "ini=",C);
        if(cs.forward(C))
        {
            lib.show(std::cerr << "forward=",C);
            std::cerr << "in #forwardCycle=" << cs.lastForwardCycles << std::endl;
        }
        else
        {
            std::cerr << "couldn't forward!" << std::endl;
        }
        
        for(size_t i=1;i<=cs.N;++i)
        {
            (void)cs.equilibria[i]->sweep(cs.K[i],C,*cs.Caux,true);
        }


        if( cs.swept(C) )
        {
            std::cerr << "swept!" << std::endl;
        }

        mkl::quark::ld(C,0);
        if( cs.swept(C) )
        {
            std::cerr << "swept!" << std::endl;
            if(cs.forward(C))
            {
                lib.show(std::cerr << "forward=",C);
                std::cerr << "in #forwardCycle=" << cs.lastForwardCycles << std::endl;
            }
            else
            {
                std::cerr << "couldn't forward!" << std::endl;
            }
        }

        mkl::quark::ld(C,0);
        if(cs.forward(C))
        {
            lib.show(std::cerr << "forward=",C);
            std::cerr << "in #forwardCycle=" << cs.lastForwardCycles << std::endl;
        }
        else
        {
            std::cerr << "couldn't forward!" << std::endl;
        }

        Boot boot;
        boot.electroneutrality(lib);
        boot.conserve(0,ah,am);
        boot.conserve(0,NH4,NH3);
        
        std::cerr << boot << std::endl;
        boot.init(lib);


    }
}
Y_UTEST_DONE()

